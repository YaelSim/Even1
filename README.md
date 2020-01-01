# Even1
Linoy and Yael - First part of project - EVEN1
Submission Date: 01/06/2020

Hello Readers!

This is EVEN1 - the first code assignment in which we cooperated and had to work with a partner.

This first shot is a part of a continuous project of our second-year Bsc studies in Computer Science at Bar-Ilan University. We have known each other since the first day of our first year, so we naturally worked together smoothly - each of us tried her best to achieve the goal and make that airplane fly! (:

This is actually our first exprience with working together as a part of a coding group.
I hope you enjoy viewing our code! So let us start...

"Engine is warming.... Let's fly!"

The project is based on several main components:
1. FlightGear Simulator - a platform that functions as a client/server (depends on requirements). Each of us installed FlightGear 2019.1.1 version to Ubuntu, Linux O.S. FlightGear contains a beautiful GUI that lets the user be a part of flying a vintage piece of aircraft.

2. Our ability to understand complexed arithmetic expressions and calculate them accordingly (via coding, of course).

3. Parsing a given .txt file into a data structure that will be used during the entire execution. Those strings will be translated later to a set of commands we'll execute one by one, each with different parameters, syntax, etc.

4. Deep understanding of programming in a multi-threaded environment.

5. Taking advantage of OOP's polymorphism. We used polymorphism in order to create a Command class that will have some sub-classes. Each type of command will be executed in a different way; For instance, the Sleep command makes the main thread sleep (Do nothing) for a required amount of milliseconds.

6. Full understand of CPP syntax, including data structures, libraries etc. This entire project is written in CPP.



Each of the components above has a critical part in EVEN1.

Lexer - a class that is responsible for reading the .txt file, dividing its' content to strings and stores them in a Vector<string>. Of course it will handle spaces, tabs, EOF-s, '\n', etc.
  
  
Parser - a class that is responsible for -simple as it is- parsing the given vector and "getting a command" out of the given strings. Each command differs in the amount and type of parameters, so each type of command is handled differently. Let us view this pseudo code-


      while (!lexVec.empty()) {
        key = lexVec.at(index);
        if (curr != nullptr) {
            index += (*curr).execute(lexVec);
        }
        // delete from lexer vector the strings that has been already used
        while (index != 0) {
            auto first = lexVec.begin();
            lexVec.erase(first);
            index--;
        }
    }


SymbolTable- a class that resembles to the computer's main memory symbol table. Each var to be declared in the .txt or the .xml files will be inserted to the symbol table. That enables us to know what is the most updated value of a var, for example. This class is implemented with 2 unordered_maps<string, VarObject*>. For instance, the add/update method is we've implemented is seprated for each map.


    void SymbolTable::actOnTxtSymbolTable(string name, VarObject* varObj) {
      if (this->txtSymbolTable.find(name) == this->txtSymbolTable.end()) {
          this->txtSymbolTable.insert({name, varObj});
      } else {
        this->txtSymbolTable.at(name)->setVarValue((*varObj).getVarValue()); 
      }
    }
    void SymbolTable::actOnXmlSymbolTable(string name, VarObject* varObj) {
     if (this->xmlSymbolTable.find(name) == this->xmlSymbolTable.end()) {
         this->xmlSymbolTable.insert({name, varObj});
     } else {
         this->xmlSymbolTable.at(name)->setVarValue((*varObj).getVarValue());
      }
    }        
    

VarObject - a data class that combines the properties of a var - value, sim path (the location of the var's value in the flightgear simulator) and arrow (defined in the .txt file).


      class VarObject {
      private:
         string _sim;
         string _arrow;
         float _varValue;
      };


Interpreter, Expression, ex1 - classes that were imported from our previous project, that are responsible for handling arithmetic expressions. For instance, if the sleep command recieves as parameter (50+90+2-10) milliseconds, we'll use our interpreter to calculate the parameter's value and determine the exact amout of time required to sleep.
This is an example of how we call the interpreter and calculate a given expression-


      Interpreter* i3 = new Interpreter();
    Expression* e6 = nullptr;
    try {
        i3->setVariables("x=-210;y=-8");
        e6 = i3->interpret("-x/70");
        std::cout << "6: " << e6->calculate() << std::endl;
        delete e6;
        delete i3;
    } catch (const char* e) {
        if (e6 != nullptr) {
            delete e6;
        }
        if (i3 != nullptr) {
            delete i3;
        }
        std::cout << e << std::endl;
    }
  
  
  Thanks for reading! You're good to go!
  We hope you've enjoyed viewing our project!
  Please contact us for further details :)

        Yael and Linoy
