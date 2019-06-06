from include.spirv.include.spirv.unified1.spirv import *

ignore = ["MagicNumber", "Version", "Revision", "OpCodeMask", "WordCountShift"]

def write_enum_map(filecpp, key):
    enum = spv[key]
    #prepare map
    toStringMap = "const std::map<spv::" + key + ", std::string> " + key +"ToStringMap = {\n"
    fromStringMap = "const std::map<std::string, spv::" + key + "> " + key +"FromStringMap = {\n"
    for name in enum:
        toStringMap += "\t{spv::" + key + "::" + name + ", \"" + name + "\"" + "},\n"
        fromStringMap += "\t{\"" + name + "\", spv::" + key + "::" + name + "},\n"
    toStringMap += "};\n\n";
    fromStringMap += "};\n\n";
    #prepare functions
    fromStringFunc = "template <>\nspv::" + key +" fromString<spv::" + key +">(std::string str)\n{\n"
    fromStringFunc += "\tauto it = " + key + "FromStringMap.find(str);\n"
    fromStringFunc += "\tif (it != " + key + "FromStringMap.end())\n"
    fromStringFunc += "\t\treturn it->second;\n"
    fromStringFunc += "\tthrow std::runtime_error(\"Unknown enum key\");\n"
    fromStringFunc += "}\n\n"

    toStringFunc = "std::string toString(spv::" + key + " key)\n{\n"
    toStringFunc += "\tauto it = " + key + "ToStringMap.find(key);\n"
    toStringFunc += "\tif (it != " + key + "ToStringMap.end())\n"
    toStringFunc += "\t\treturn it->second;\n"
    toStringFunc += "\tthrow std::runtime_error(\"Unknown enum key\");\n"
    toStringFunc += "}\n\n"
    #write
    filecpp.write(toStringMap)
    filecpp.write(fromStringMap)
    filecpp.write(toStringFunc)
    filecpp.write(fromStringFunc)


def write_enum_function(filehpp, key):
    enum = spv[key]
    filehpp.write("template <>\nspv::" + key + " fromString<spv::" + key + ">(std::string str);\n\n")
    filehpp.write("std::string toString(spv::" + key + ");\n\n")

namecpp = "src/gen/spirv_enums.cpp"
namehpp = "src/gen/spirv_enums.hpp"

with open(namecpp, "w") as filecpp, open(namehpp, "w") as filehpp:
    filehpp.write("#ifndef SPIRV_ENUM\n")
    filehpp.write("#define SPIRV_ENUM\n\n")

    filehpp.write("#include <string>\n")
    filehpp.write("#include <map>\n")
    filehpp.write("#include <spirv.hpp11>\n\n")

    filecpp.write("#include \"spirv_enums.hpp\"\n")
    filecpp.write("#include <string>\n")
    filecpp.write("#include <map>\n")
    filecpp.write("#include <spirv.hpp11>\n\n")

    filehpp.write("template <class TEnum>\nTEnum fromString(std::string str);\n\n")

    for key in spv:
        if key in ignore:
            continue
        write_enum_map(filecpp, key)
        write_enum_function(filehpp, key)

    filehpp.write("#endif // SPIRV_ENUM\n")
