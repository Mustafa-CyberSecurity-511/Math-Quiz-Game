#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
using namespace std;

// Function to ask the player how many questions they want (1 to 10)
short HowManyQuestion()
{
    short Questions = 0;
    do
    {
        cout << "How Many Question Do you Want from 1 to 10 ? ";
        cin >> Questions;
    } while (Questions < 1 || Questions > 10); // Loop until valid input
    return Questions;
}

// Enum for question difficulty levels
enum enQuestionLevel
{
    Easy = 1,
    Mid,
    Hard,
    Mix
};

// Function to read the level of questions from the user
enQuestionLevel ReadQuestionLevel()
{
    short Choice = 0;
    do
    {
        cout << "Enter Level [1]:Easy , [2]:Mid , [3]:Hard , [4]:Mix : ";
        cin >> Choice;
    } while (Choice < 1 || Choice > 4); // Loop until valid input
    return static_cast<enQuestionLevel>(Choice);
}

// Enum for types of arithmetic operations
enum enOperationType
{
    Add = 1,
    Sub,
    Mult,
    Div,
    MixOP
};

// Function to read the operation type from the user
enOperationType ReadOperationType()
{
    short OpType = 0;
    do
    {
        cout << "Enter Operation Type [1]:Add , [2]:Sub , [3]:Mult , [4]:Div , [5]:MixOP : ";
        cin >> OpType;
    } while (OpType < 1 || OpType > 5); // Loop until valid input
    return static_cast<enOperationType>(OpType);
}

// Structure to represent a single question
struct stQuesion
{
    int Number1 = 0;
    int Number2 = 0;
    int CorrectAnswer = 0;  // Store correct answer
    int PlayerAnswer = 0;   // Store player's answer
    bool AnswerResult = false; // True if player answered correctly
    enQuestionLevel Level;      // Difficulty level
    enOperationType Operation;  // Operation type
};

// Generate a random number between two values
int RandomNumber(int From, int To)
{
    return rand() % (To - From + 1) + From;
}

// Get a random arithmetic operation
enOperationType GetRandomOperation()
{
    return static_cast<enOperationType>(RandomNumber(1, 4));
}

// Function to calculate the result of two numbers based on the operation
int SimpleCalculator(int a, int b, enOperationType OpType)
{
    switch (OpType)
    {
    case enOperationType::Add:
        return a + b;
    case enOperationType::Sub:
        return a - b;
    case enOperationType::Mult:
        return a * b;
    case enOperationType::Div:
        if (b == 0)
            throw std::invalid_argument("Division by Zero !"); // Error handling
        return a / b;
    default:
        throw std::invalid_argument("Unknown Operation");
    }
}

// Generate a question based on difficulty level and operation type
stQuesion GenerateQuestions(enOperationType OpType, enQuestionLevel QuestionLevel)
{
    stQuesion Quesion;

    // If user selected Mix level, choose a random level
    if (QuestionLevel == enQuestionLevel::Mix)
        QuestionLevel = static_cast<enQuestionLevel>(RandomNumber(1, 3));

    // If user selected Mix operation, choose a random operation
    if (OpType == enOperationType::MixOP)
        OpType = GetRandomOperation();

    Quesion.Level = QuestionLevel;
    Quesion.Operation = OpType;

    int from = 1, to = 10;

    // Adjust number range based on difficulty level
    if (QuestionLevel == enQuestionLevel::Mid)
        from = 10, to = 50;
    else if (QuestionLevel == enQuestionLevel::Hard)
        from = 50, to = 100;

    // Generate two random numbers
    Quesion.Number1 = RandomNumber(from, to);
    Quesion.Number2 = RandomNumber(from, to);

    // Calculate the correct answer
    Quesion.CorrectAnswer = SimpleCalculator(Quesion.Number1, Quesion.Number2, Quesion.Operation);

    return Quesion;
}

// Return the symbol of an operation for display
string GetOperatorSymbol(enOperationType OperationType)
{
    switch (OperationType)
    {
    case enOperationType::Add:
        return "+";
    case enOperationType::Sub:
        return "-";
    case enOperationType::Mult:
        return "*";
    case enOperationType::Div:
        return "/";
    default:
        return "Mix";
    }
}

// Print a single question to the player
void PrintQuestion(const stQuesion& q, int index, int total)
{
    cout << "\nQueston [" << index + 1 << "/" << total << "]\n";
    cout << q.Number1 << " " << GetOperatorSymbol(q.Operation) << " " << q.Number2 << " = ? \n";
}

// Structure to store the quiz state
struct stQuizz
{
    stQuesion QuesionList[100]; // Array of questions
    enQuestionLevel QuestionLevel;
    enOperationType OperationType;

    short TotalQuestion{};
    short CorrectAnswer{};
    short WrongAnswer{};
    bool IsPass{};
};

// Generate all questions for the quiz
void GenarateTheQuizzQuestion(stQuizz& Quizz)
{
    for (int i = 0; i < Quizz.TotalQuestion; ++i)
    {
        Quizz.QuesionList[i] = GenerateQuestions(Quizz.OperationType, Quizz.QuestionLevel);
    }
}

// Change console color based on correct/incorrect answer
void SetScreenColor(bool Right)
{
    if (Right)
        system("color 2F"); // Green for correct
    else
    {
        system("color 4F"); // Red for incorrect
        cout << "\a";        // Beep sound
    }
}

// Check if player's answer is correct and update quiz stats
void CorrectAnswer(stQuizz& Quizz, int Index)
{
    stQuesion& q = Quizz.QuesionList[Index];

    if (q.PlayerAnswer == q.CorrectAnswer)
    {
        q.AnswerResult = true;
        Quizz.CorrectAnswer++;
        cout << "Correct Answer :-)\n";
    }
    else
    {
        q.AnswerResult = false;
        Quizz.WrongAnswer++;
        cout << "Wrong Answer :-(\n";
        cout << "Correct Answer was: " << q.CorrectAnswer << endl;
    }

    SetScreenColor(q.AnswerResult); // Change console color based on result
}

// Get player's answer input
int GetAnswerFromUser()
{
    int answer = 0;
    cout << "Your Answer: ";
    cin >> answer;
    return answer;
}

// Ask all questions in the quiz
void AskQuestions(stQuizz& Quizz)
{
    for (int i = 0; i < Quizz.TotalQuestion; ++i)
    {
        PrintQuestion(Quizz.QuesionList[i], i, Quizz.TotalQuestion);
        Quizz.QuesionList[i].PlayerAnswer = GetAnswerFromUser();
        CorrectAnswer(Quizz, i);
        cout << "-------------------------\n";
    }

    // Player passes if correct answers >= wrong answers
    Quizz.IsPass = (Quizz.CorrectAnswer >= Quizz.WrongAnswer);
}

// Convert level enum to string for display
string GetLevelText(enQuestionLevel Level)
{
    switch (Level)
    {
    case enQuestionLevel::Easy:
        return "Easy";
    case enQuestionLevel::Mid:
        return "Medium";
    case enQuestionLevel::Hard:
        return "Hard";
    case enQuestionLevel::Mix:
        return "Mix Level";
    }
    return "Unknown";
}

// Print final results of the quiz
void PrintFinalResults(const stQuizz& Quizz)
{
    cout << "\n========== Final Result ==========\n";
    cout << "Result: " << (Quizz.IsPass ? "PASS :-)" : "FAIL :-(") << "\n";
    cout << "Level: " << GetLevelText(Quizz.QuestionLevel) << "\n";
    cout << "Operation: " << GetOperatorSymbol(Quizz.OperationType) << "\n";
    cout << "Correct Answers: " << Quizz.CorrectAnswer << "\n";
    cout << "Wrong Answers: " << Quizz.WrongAnswer << "\n";
    cout << "==================================\n";
}

// Clear console screen and reset color
void ResetScreen()
{
    system("cls");  // Clear screen for Windows
    system("color 0F"); // Default console color
    // system("clear"); // Uncomment for Linux
}

// Main function to play a single quiz
void PlayQuizzGame()
{
    stQuizz Quizz;
    Quizz.TotalQuestion = HowManyQuestion();
    Quizz.QuestionLevel = ReadQuestionLevel();
    Quizz.OperationType = ReadOperationType();

    GenarateTheQuizzQuestion(Quizz);
    AskQuestions(Quizz);
    PrintFinalResults(Quizz);
}

// Start the game and ask if the player wants to play again
void StartGame()
{
    char again = 'Y';
    do
    {
        ResetScreen();
        PlayQuizzGame();
        cout << "\nDo you want to play again? (y/n): ";
        cin >> again;
    } while (again == 'Y' || again == 'y');
}

// Program entry point
int main()
{
    srand(time(0)); // Seed random number generator
    StartGame();
    return 0;
}
