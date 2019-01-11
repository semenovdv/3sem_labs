import gedcom

gc = gedcom.parse("tree.ged")

with open("prog.pro", 'w') as output:
    for person in gc.individuals:
        firstname, lastname = person.name
        if(person.father):
            output.write("{0}{1}, {2}{3}\n".format("father(", person.father.name[0], person.name[0], ")"))
        if(person.mother):
            output.write("{0}{1}, {2}{3}\n".format("mother(", person.mother.name[0], person.name[0], ")"))
