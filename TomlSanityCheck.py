import toml
import os
import sys
import argparse
from abc import ABC, abstractmethod

class TomlSanityCheck:
    def __init__(self, file_path):
        self.config_file = toml.load(file_path)

    def validate(self, sections: list=[], verbose: bool=False):
# Make sure that dictionary has correct formatting. For reference, the file formatted as follows:
# Each parameter is formatted like: [XXX.YYY] where XXX refers to the group of the parameter and YYY refers to the name
# Each parameter has exactly two key:value pairs.
#   The first is the actual value of the parameter (a path, a number, a string etc.)
#   The second is the sanity check/constraint on the parameter. See TomlSanityCheck.py for a current list of supported constraints
        if (len(sections)==0):
            sections = [sec for sec in self.config_file]
        for section_name in sections:
            if(verbose):
                print(section_name)
            try:
                section = self.config_file[section_name]
            except:
                raise Exception(section +" is not found in config file!")
            self.recursive_val(section,verbose,parent_nodes=[section_name])

    def recursive_val(self,cur_tree: dict, verbose: bool=False, parent_nodes: list= [], level: int=1):
    # Base case/ Allows for empty sections
        if(len(cur_tree)==0):
            return True
    # Possible Base case
        elif(len(cur_tree)==2): # There is only two key-value pairs at this level. Check if it's a terminating node
            non_dict_nodes_found  = 0
            for section_header, value in cur_tree.items():
                if(type(value) is dict): # This is a dictionary. We need to keep traversing the tree
                    level += 1
                    parent_nodes.append(section_header)
                    self.recursive_val(value,verbose,parent_nodes,level)
                    level -=1
                    parent_nodes.pop()
                else:
                    non_dict_nodes_found += 1
            if(non_dict_nodes_found != 2) and (non_dict_nodes_found != 0): #Either they should all be dictionaries, or they should all not be 
                err_msg = "File Malformatted. Only one non-dict found. Problem in the following header '"+str(parent_nodes)
                raise Exception(err_msg)
            if(non_dict_nodes_found == 2):
            #If you passed the above checks, then cur_tree is a dictionary of length two, and there are no sub-dictionaries. Hence, you can try and validate it
                self.parse_leaves(cur_tree, parent_nodes, verbose)
            return True
        else: # The only allowed nodes here are those that only have sub-nodes. So if a value is not a dictionary, then throw an error
            for section_header, value in cur_tree.items():
                if(not (type(value) is dict)): # This is not a dictionary. Since there aren't two sub-nodes, the file isn't properly formatted. Throw error
#                    parent_nodes.append(section_header)
                    err_msg = "File Malformatted. Problem in the following header '"+str(parent_nodes)
                    print(cur_tree)
                    raise Exception(err_msg)
                else: # This is a dictionary. Recurse on it
                    level += 1
                    parent_nodes.append(section_header)
                    self.recursive_val(value,verbose,parent_nodes,level)
                    level -=1
                    parent_nodes.pop()

    def parse_leaves(self, leaf_tree: dict, parent_nodes=[], verbose=False):
        # Check if k-v pairs in leaf_tree are correct
        if(verbose):
            print(leaf_tree)
        val = None
        constraint_name = None
        try:
            val = leaf_tree["value"]
        except:
            err_msg = "Leaf node malformatted. 'Val' missing. Problem in the following header '"+str(parent_nodes)
            raise Exception(err_msg)
        try:
            constraint_name = leaf_tree["constraint"]
        except:
            err_msg = "Leaf node malformatted. 'constraint' missing. Problem in the following header '"+str(parent_nodes)
            raise Exception(err_msg)
        constraint = None
        match constraint_name:
            case "None":
                constraint = NoConstraint(val)
            case "NonEmptyString":
                constraint = NonEmptyString(val)
            case "SPosInt":
                constraint = StrictPositiveInt(val)
            case "PosInt":
                constraint = PositiveInt(val)
            case "SPosFloat":
                constraint = StrictPositiveFloat(val)
            case "PosFloat":
                constraint = PositiveFloat(val)
            case "Floating":
                constraint = Floating(val)
            case "ValidFolder":
                constraint = ValidFolder(val)
            case "DetectorGeometry":
                constraint = DetectorGeometry(val)
            case "Boolean":
                constraint = BooleanCheck(val)
            case "ScaleCheck":
                constraint = ScaleCheck(val)
            case "EnergyDist":
                constraint = EnergyDist(val)
            case "TrainingTarget":
                constraint = TrainingTarget(val)
            case "TrainingModel":
                constraint = TrainingModel(val)
        if not constraint:
            err_msg = "Constraint '"+constraint_name + "' is currently not defined."
            err_msg += " Did you create a inherited class, and add the constraint to the above match statement?"
            raise Exception(err_msg)
        if not constraint.validate():
            err_msg = "section '"+str(parent_nodes)+"' with value '"+str(val)+"'"
            err_msg += " doesn't satisfy constraint '"+str(constraint_name)+"'"
            raise Exception(err_msg)

    def emit_config(self, filename):
    ## Saves current internal AST of toml file as a new toml file, and returns the string of the file
        with open(filename, 'w') as f:
            output = toml.dump(self.config_file, f)
        return output

    def traverse_tree_bash(self,cur_tree: dict, parent_nodes: list= [], verbose: bool=False, level=0):
        if(verbose):
            print(level, parent_nodes)
        if (len(cur_tree) == 0):
            return ""
        elif(len(cur_tree) == 2):
            str_found = 0
            output = ""
            for header,sub_tree in cur_tree.items():
                if(type(sub_tree) is dict):
                    parent_nodes.append(header)
                    level += 1
                    output += self.traverse_tree_bash(sub_tree, parent_nodes, verbose, level)
                    level -= 1
                    parent_nodes.pop()
                    return output
                else:
                    str_found += 1
            # Assumes that keys are {"value","constraint"}
            if(str_found ==0):
                return output
            else:
                bash_var = "_"
                for node_name in parent_nodes:
                    bash_var += node_name+"_"
                bash_var = bash_var[1:-1]
                bash_var = bash_var + "="+ str(cur_tree["value"])+ "\n"
                if(verbose):
                    print(bash_var)
                output += bash_var
        else:
            output = ""
            for header,sub_tree in cur_tree.items():
                    parent_nodes.append(header)
                    level += 1
                    output += self.traverse_tree_bash(sub_tree, parent_nodes, verbose, level)
                    level -= 1
                    parent_nodes.pop()
        return output

    def gen_bash_variables(self,sections = [], verbose=False):
    # Assumes that .toml file has undergone validation
        output = ""
        if (len(sections)==0):
            sections = [sec for sec in self.config_file]
        for section_name in sections:
            try:
                section = self.config_file[section_name]
            except:
                raise Exception(section +" is not found in config file!")
            output += self.traverse_tree_bash(section, [section_name], verbose)
        return output
# Abstract base class from which all other constraints are derived from
class TOMLParameterConstraint(ABC):
    def __init__(self,parameter_value):
        self.value = parameter_value
# We expect the method to return a boolean stating weather self.value satisfies the constraint
    @abstractmethod
    def validate(self):
        pass

class NoConstraint(TOMLParameterConstraint):
# Checks for non-empty string as value
    def __init__(self, parameter_value):
        super(NoConstraint, self).__init__(parameter_value)
    def validate(self):
        return True

class NonEmptyString(TOMLParameterConstraint):
# Checks for non-empty string as value
    def __init__(self, parameter_value):
        super(NonEmptyString, self).__init__(parameter_value)
    def validate(self):
        if not isinstance(self.value, str):
            return False
        if len(self.value) == 0:
            return False
        return True

class StrictPositiveInt(TOMLParameterConstraint):
# Checks for non-empty string as value
    def __init__(self, parameter_value):
        super(StrictPositiveInt, self).__init__(parameter_value)
    def validate(self):
        if not isinstance(self.value,int):
            return False
        try:
            ge = (self.value > 0)
        except:
            return False
        return ge

class PositiveInt(TOMLParameterConstraint):
# Checks for non-empty string as value
    def __init__(self, parameter_value):
        super(PositiveInt, self).__init__(parameter_value)
    def validate(self):
        if not isinstance(self.value,int):
            return False
        try:
            ge = (self.value >= 0)
        except:
            return False
        return ge

class StrictPositiveFloat(TOMLParameterConstraint):
# Checks for non-empty string as value
    def __init__(self, parameter_value):
        super(StrictPositiveFloat, self).__init__(parameter_value)
    def validate(self):
        if not isinstance(self.value,float):
            return False
        try:
            ge = (self.value > 0)
        except:
            return False
        return ge

class PositiveFloat(TOMLParameterConstraint):
# Checks for non-empty string as value
    def __init__(self, parameter_value):
        super(PositiveFloat, self).__init__(parameter_value)
    def validate(self):
        if not isinstance(self.value,float):
            return False
        try:
            ge = (self.value >= 0)
        except:
            return False
        return ge

class Floating(TOMLParameterConstraint):
# Checks for non-empty string as value
    def __init__(self, parameter_value):
        super(Floating, self).__init__(parameter_value)
    def validate(self):
        if not isinstance(self.value,float):
            return False
        return True

class ValidFolder(TOMLParameterConstraint):
# Checks for non-empty string as value
    def __init__(self, parameter_value):
        super(ValidFolder, self).__init__(parameter_value)
    def validate(self):
        if os.path.isdir(self.value):
            return True
        return False

class DetectorGeometry(TOMLParameterConstraint):
# Checks for non-empty string as value
    def __init__(self, parameter_value):
        super(DetectorGeometry, self).__init__(parameter_value)
    def validate(self):
        valid_geometries = ["cube","flat"]
        lowered = self.value.lower()
        if lowered not in valid_geometries:
            return False
        return True

class BooleanCheck(TOMLParameterConstraint):
# Checks for non-empty string as value
    def __init__(self, parameter_value):
        super(BooleanCheck, self).__init__(parameter_value)
    def validate(self):
        if isinstance(self.value,bool):
            return True
        return False

class ScaleCheck(TOMLParameterConstraint):
# Checks for non-empty string as value
    def __init__(self, parameter_value):
        super(ScaleCheck, self).__init__(parameter_value)
    def validate(self):
        if((self.value == "linear") or (self.value == "log")):
            return True
        else:
            return False

class EnergyDist(TOMLParameterConstraint):
# Checks for non-empty string as value
    def __init__(self, parameter_value):
        super(EnergyDist, self).__init__(parameter_value)
    def validate(self):
        if((self.value == "PowerLaw") or (self.value == "Uniform")):
            return True
        else:
            return False

class TrainingTarget(TOMLParameterConstraint):
    def __init__(self, parameter_value):
        super(TrainingTarget,self).__init__(parameter_value.lower().strip())
    def validate(self):
        if((self.value == "class") or (self.value == "energy")):
            return True
        else:
            return False

class TrainingModel(TOMLParameterConstraint):
    def __init__(self, parameter_value):
        super(TrainingModel,self).__init__(parameter_value.lower().strip())
    def validate(self):
        if((self.value == "simple") or (self.value == "cnn")):
            return True
        else:
            return False

if __name__ =="__main__":
# By Default, converts toml file to a list of bash variables
    parser = argparse.ArgumentParser(prog='CreateMCNNData')
    parser.add_argument('GenDataTOML',help="Path to .toml config file to generate data")
    parser.add_argument("-v","--verbose",action='store_true', help="Verbosity")
    parser.add_argument("-s", "--section" ,type=str, help="Comma delimited list stating which section(s) to read in. If nothing, just reads and validates everything")
    args = parser.parse_args()
    if args.section:
    # Splits section argument into constituent parts, removes all whitespace in each part, then creates list
        sections = [''.join(item.split()) for item in args.section.split(',')]
    else:
        sections = []
    s = TomlSanityCheck(args.GenDataTOML)
    s.validate(sections, False)
    # Assuming all went well, print out a string containing bash variables
    print(s.gen_bash_variables(sections, args.verbose))