from include.spirv.include.spirv.unified1.spirv import *

ignore = ["MagicNumber", "Version", "Revision", "OpCodeMask", "WordCountShift"]

def write_enum_map(file, key):
    enum = spv[key]
    toStringMap = "const std::map<spv::" + key + ", std::string> toStringMap = {\n"
    fromStringMap = "const std::map<std::string, spv::" + key + "> toStringMap = {\n"
    for name in enum:
        toStringMap += "{spv::" + key + "::" + name + ", \"" + name + "\"" + "},\n"
        fromStringMap += "{\"" + name + "\", spv::" + key + "::" + name + "},\n"
    toStringMap += "};\n\n";
    fromStringMap += "};\n\n";
    file.write(toStringMap)
    file.write(fromStringMap)

with open("src/gen/spirv_enums.hpp", "w") as file:
    file.write("#ifndef SPIRV_ENUM\n")
    file.write("#define SPIRV_ENUM\n\n")

    file.write("#include <string>\n")
    file.write("#include <map>\n")
    file.write("#include <spirv.hpp11>\n\n")

    for key in spv:
        if key in ignore:
            continue
        write_enum_map(file, key)
    file.write("#endif // SPIRV_ENUM\n")

exit(0)
