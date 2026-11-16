print("INTELLIGENT CHATBOT")

# PEAS Representation
print("\nPEAS Representation:")
print("Performance: Correct responses")
print("Environment: Chat window")
print("Actuators: Display messages")
print("Sensors: User input")

# Intelligent Agent
while True:

    user = input("\nYou: ").lower()

    if user == "hello":
        print("Bot: Hello! How are you?")

    elif user == "how are you":
        print("Bot: I am fine!")

    elif user == "your name":
        print("Bot: I am a Simple Chatbot.")

    elif user == "bye":
        print("Bot: Goodbye!")
        break

    else:
        print("Bot: Sorry, I don't understand.")