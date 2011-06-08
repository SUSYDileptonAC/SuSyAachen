'''
Created on 18.08.2009

@author: heron
'''
#-------------  need to put this in its own package some time
import ConfigParser
import re
class BetterConfigParser(ConfigParser.ConfigParser):

    def __init__(self, defaults=None):#in python 2.6, dict_type=dict):
        self.__runtimeRepMap = {}
        self._activeSection = None
        ConfigParser.ConfigParser.__init__(self, defaults)#, dict_type)            

    def get(self, section, option, raw = True, default = None, sectionwideReplace = True):
        result = None
        matchedName = None
        for name, item in self.items(section, raw=raw):
            if re.match(name+"$", option) != None:
                assert matchedName == None, "option '%s' matches '%s' and '%s'"%(option, matchedName, name)
                matchedName = name
                result = item
        if matchedName == None:
            if not default is None:
                result = default
            if self.has_option(section, "default"):
                result = self.get(section, "default", raw=raw)
        if sectionwideReplace and "splitlines" in dir(result):    
            result = "\n".join([self.__replaceSectionwideTemplates(l) for l in result.splitlines()])
            result = result.replace("\<", "<")
        #not sure if I have created achicken egg problem here
        for entry in self.__runtimeRepMap:
            if "replace" in dir(result):
                result = result.replace(".oO[%s]Oo." % entry, self.__runtimeRepMap[entry])
            if "__setitem__" in dir(result):
                for i in range(len(result)):
                    if "replace" in dir(result[i]):
                        result[i] = result[i].replace(".oO[%s]Oo." % entry, self.__runtimeRepMap[entry])
        return result
    
# need to look this syntax up once I am back online...
#    def items(self, **opts):
#        opts["raw"] = True
#        return ConfigParser.ConfigParser.itmes(self, **opts)
    
    def optionxform(self, optionstr):
        '''enable case sensitive options in .ini files'''
        return optionstr

    def write(self, fileObject):
        for section in self.sections():
            for option in self.options(section):
                self.set(section, option, self.get(section,option, raw=True, sectionwideReplace=False))
        ConfigParser.ConfigParser.write(self, fileObject)

    def addRuntimeReplacement(self, entry, replacement):
        if entry in self.__runtimeRepMap:
            raise StandardError, "'%s' is already in the runtime replacements. You are not allowed to modify!" % entry
        self.__runtimeRepMap[entry] = replacement

    def __replaceSectionwideTemplates(self, data):
        '''replace <section|option> with get(section,option) recursivly'''
        result = data
        findExpression = re.compile("((.*)\<(?<!\\\<)([^\>]*)\|([^\>]*)\>(.*))*")
        groups = findExpression.search(data).groups()
        sectionName = groups[2]
        if not self._activeSection == None and sectionName == "activeSection":
            sectionName = self._activeSection
        if not self.has_section(sectionName) and not sectionName == None:
            print "WARNING: skipping '%s' in '%s' because section could not be found"%(sectionName, data)
        if not groups == (None, None, None, None, None) and self.has_section(sectionName): # expression not matched
            result = self.__replaceSectionwideTemplates(groups[1])
            
            matchedName = None
            for name, item in self.items(sectionName, raw=True):
                if not "$" in name: name+="$"
                if re.match(name, groups[3]) != None:
                    if item == "passedString": result ="%s%s"%(result,  self.__replaceSectionwideTemplates(groups[3]))
                    else: result ="%s%s"%(result,  self.__replaceSectionwideTemplates(item))                        
                    assert matchedName == None, " '<%s|%s>' did not only match '%s' but also '%s'"%(sectionName,groups[3], name, matchedName)
                    matchedName = name
                    
            if matchedName == None:
                if self.has_option(sectionName,"default"):
                    if self.get(sectionName, "default") == "passedString":
                        result ="%s%s"%(result, self.__replaceSectionwideTemplates( groups[3] ))
                    else:
                        result ="%s%s"%(result, self.__replaceSectionwideTemplates( self.get(sectionName, "default") ))
                else:
                    #rethrow exception for the poor
                    assert self.has_option(sectionName, groups[3]), "could not find '%s' in section '%s' needed in '%s'"%(groups[3], sectionName, data)

                    result ="%s%s"%(result, self.get(sectionName, groups[3]))
            result ="%s%s"%(result, self.__replaceSectionwideTemplates(groups[4]))
        if not findExpression.match(result).groups() ==  (None, None, None, None, None): result = self.__replaceSectionwideTemplates(result)
        assert not result == None, "something went wrong convertig '%s'"%data
        return result
    
    def setActiveSection(self, name=None):
        from copy import deepcopy
        before = deepcopy(self._activeSection)
        self._activeSection = name
        return before

class Section:
    def __init__(self, config, name, defaults = {}):
        self._config = config
        self._name = name
        self._defaults = {"defaultSections":[]} 
        self.setDefaults(defaults)
        self.evalGlobals = {}
    
    def setDefaults(self, defaults):
        for name, newValue in defaults.iteritems():
                #print self._defaults[name], newValue
            if name in self._defaults:
                self._defaults[name] = self.__convert(newValue, self._defaults[name])
            else:
                self._defaults[name] = newValue
                
    def __str__(self):
        result ="Section: '%s'\n"%self._name 
        for i in self._config.options(self._name):
            result += "%s = %s\n"%(i, getattr(self,i))
        for defaultSection in self.defaultSections:
            result +="From Default '%s'\n"%defaultSection
            for i in self._config.options("default:%s"%defaultSection):
                result += "%s = %s\n"%(i, getattr(self,i))
        return result
    
    def __convert(self, read, default):
        result = None
        if type(default) in [float, int]:
            result = eval(read, self.evalGlobals)
        elif type(default) in [list, bool]:
            try:
                result = eval(read, self.evalGlobals)
                
            except:
                result = read.split()
                for i in range(len(result)):
                    if i < len(default):
                        result[i] =  self.__convert(result[i], default[i])
                    else:   
                        if len(default) > 0:
                            result[i] =  self.__convert(result[i], default[-1])
        else:
            result = type(default)(read)
        return result
    
    def __getattr__(self, name):
        if name in self.__dict__:
            return self.__dict__[ name ]
        else:
            assert self._name in self._config.sections(), "Could not find section '%s' while getting '%s'"%(self._name, name)
            activeBefore = self._config.setActiveSection(self._name)
            if not name =="defaultSections":
                for defaultSection in self.defaultSections:
                    if not self._config.get("default:%s"%defaultSection, name) == None: 
                        self.setDefaults({name:self._config.get("default:%s"%defaultSection, name)})
            #self._defaults.update( eval(self._config.get(self._name, "defaults", default = "{}") ) )
            default = None
            if name in self._defaults: default = self._defaults[name]
            result = self._config.get(self._name, name, default = default)
            
            if type(result) == type(""):
                if not name in self._defaults or self._defaults[name] == None:
                    pass                        
                else:  
                    result = self.__convert(result, self._defaults[name])
            self._config.setActiveSection(activeBefore)
            return result

def orderByExpressions(input, expressions, sortFkt = sorted):
    from re import match
    result = []
    for orderExpr in expressions:
        for element in input:
            expr = orderExpr
            if not "$" in orderExpr:
                expr +="$"
            if not match(expr, element) == None:
                result.append( element )
    for element in sortFkt(input):
        if not element in result:
            result.append(element)
    return result
        
import unittest
class Test(unittest.TestCase):
    def setUp(self):
        file = open("helpers.unittest.ini","w")
        file.write("""

[default:testBase]
a = <other|aNumber>
b = hallo

[default:testSpecial]
a = 2 
c = <other|<activeSection|name>>

[other]
default = config default
.*Match = matches
direct = direct
aNumber = 1
.*String = mit sauce  

[emptySection]
nothing = here

[test]
defaultSections = testBase
runtimeReplace = replace is: '.oO[replaceMe]Oo.'
sectionReplace = direct:'<other|direct>' default:'<other|triggerDefault>' match:'<other|triggerMatch>'
basicStuff = some things 
number = 1
floatNumber = 1.1
floatList = [1.1, 2.3]

[test2]
defaultSections = testBase testSpecial
name = someString

""")
        file.close()
        self.cfg = BetterConfigParser()
        self.cfg.read("helpers.unittest.ini")
        
    def tearDown(self):
        from os import remove
        remove('helpers.unittest.ini')
        
    def testBasic(self):
        self.assertTrue(self.cfg.get("test","basicStuff")=="some things")
        self.assertTrue(self.cfg.get("other","doesItMatch", default="not used at all")=="matches")
        self.assertTrue(self.cfg.get("other","notEvenThere", default="direct default")=="config default")
        self.assertTrue(self.cfg.get("emptySection","notEvenThere", default="direct default")=="direct default")
        
    def testRuntimeReplace(self):
        self.cfg.addRuntimeReplacement("replaceMe", "replaced.")
        self.assertTrue(self.cfg.get("test","runtimeReplace")=="replace is: 'replaced.'")
        
    def testSectionReplace(self):
        self.assertTrue(self.cfg.get("test","sectionReplace")=="direct:'direct' default:'config default' match:'matches'")
    
    def testSection(self):
        defaults = {
                    "number":0,
                    "anotherNumber":42,
                    "floatNumber":0.,
                    "floatList":[0.,]
                    }
        sec = Section(self.cfg, "test", defaults)
        self.assertTrue(sec.number == 1 and type(sec.number) == int)
        self.assertTrue(sec.anotherNumber == 42 and type(sec.anotherNumber) == int)
        self.assertTrue(sec.floatNumber == 1.1 and type(sec.floatNumber) == float)
        self.assertTrue(sec.floatList == [1.1,2.3])
        self.assertTrue(sec.a == "1")
        self.assertTrue(sec.b == "hallo")
        
        sec = Section(self.cfg, "test2", defaults)
        self.assertTrue(sec.number == 0 and type(sec.number) == int)
        self.assertTrue(sec.anotherNumber == 42 and type(sec.anotherNumber) == int)
        self.assertTrue(sec.floatNumber == 0. and type(sec.floatNumber) == float)
        self.assertTrue(sec.floatList == [0.])
        self.assertTrue(sec.a == "2")
        self.assertTrue(sec.b == "hallo")
        self.assertTrue(sec.c == "mit sauce")
    
    
    
