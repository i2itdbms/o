from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split
from sklearn.tree import DecisionTreeClassifier
from sklearn.naive_bayes import GaussianNB
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix

data = load_iris()
x = data.data
y = data.target

x_train,x_test,y_train,y_test = train_test_split(
    x,y,test_size=0.2,random_state=46
)

dt = DecisionTreeClassifier(random_state=42)
dt.fit(x_train,y_train)
pred1 = dt.predict(x_test)

nb = GaussianNB()
nb.fit(x_train,y_train)
y_pred2 = nb.predict(x_test)

print("DT Accuracy store : ",accuracy_score(y_test,pred1))
print("NB Accuracy store : ",accuracy_score(y_test,y_pred2))


print("Confussion matrix of dt " , confusion_matrix(y_test,pred1))
print("Confussion matrix of nb " , confusion_matrix(y_test,y_pred2))

print("DT classification")
print(classification_report(y_test,pred1))

print("nb classification")
print(classification_report(y_test,y_pred2))


