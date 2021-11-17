print("Gimme data\n")
#########################
data = input()
#TODO

#support funcs:
def FinCheck(lst1, lst2): #input example: < ID9:  2 : 4,5 : 9 >
    sum = 0
    for ele in range(0, len(lst2)):
        sum = sum + lst1[ele] #calculate sum
    if(lst1[1] == len(lst2) & sum == lst1[2]): #print result
        print("client.sent(", lst1[1] , ",")
        print(*lst2, sep = ", ")
        print(")")
    else:
        print(lst1[0]," error")



#st1: ignore all before 1st "!"
f_idx = data.find("!")
data = data[f_idx::]
print("step1:", data, "\n")


#st2: split multiple inputs
list1 = data[1:-1].split("!")
print("St2 Input list:\n")
print(list1)

#st3: process each inputs
for i in range(len(list1)):

    l1 = list1[i].split(":") #separate input parts
    #check ID format, correct input have 4 parts
    len_l1=len(l1)
    if len_l1<=1: #no ID
        print("error")
    elif len_l1 != 4: #ID with error
        print(l1[0]," error")
    else: #final check
        l2 = l1[1].split(",") #split 2nd part in input
        FinCheck(l1,l2)
        print("\n")

print("proc end")


