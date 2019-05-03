class_list = [
	"Common",
]

def to_file_name(class_name):
	file_name = ""
	for symbol in class_name:
		if symbol.isupper():
			if len(file_name):
				file_name += "_" + symbol.lower()
			else:
				file_name += symbol.lower()
		else:
			file_name += symbol
	return file_name
		

for class_name in class_list:
	full_name = "Parser" + class_name
	file_name = to_file_name(full_name)
	guard_name = file_name.upper()
	with open(file_name + ".hpp", "w") as header_file:
		header_file.write("#ifndef " + guard_name + "\n")
		header_file.write("#define " + guard_name + "\n")
		header_file.write("\n")
		header_file.write("class " + full_name + "\n")
		header_file.write("{\npublic:\n};")
		header_file.write("\n\n")
		header_file.write("#endif")
	with open(file_name + ".cpp", "w") as source_file:
		source_file.write("#include \"" + file_name + ".hpp\"\n")