fact = ['flu','cough','fever']
rules ={
    'flu' :['fever','cough'],
    'Doctor_visit':['flu']
}

print("forward")
for result,conditions in rules.items():
    if all(c in fact for c in conditions):
        fact.append(result)
        print(result)

def backward(goal):
    if goal in fact:
        return True
    if goal in rules:
        return all(backward(c) for c in rules[goal])
    return False

print("Backward Chaining:")
print("Visit doctor:", backward("Doctor_visit"))
