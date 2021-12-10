#include <iostream>
#include <stdlib.h>
#include <string>
using namespace std;

class Node //class of the element of the string
{
private:

public:
    string data; //data in the node
    Node* next; //pointer to the next node
    Node(string newdata)
    {
        data = newdata; //the initialized data is set
        next = NULL; //the next node is set as 'NULL', as there no next node
    }
};


class postfix_string
{
private:
    Node* head; //begining of the string
    void insert_first(string newData) //inserts first element in the string in postfix form
    {
        Node* newNode = new Node(newData);
        head = newNode;
    }
    int prec(string str) //Function to return precedence of operators
    {
        if ((str == "^") || (str == "cos") || (str == "sin") || (str == "tg")
            || (str == "ctg") || (str == "ln") || (str == "log") || (str == "sqrt") || (str == "abs"))
            return 3;
        else if ((str == "/") || (str == "*"))
            return 2;
        else if ((str == "+") || (str == "-"))
            return 1;
        else
            return -1;
    }
    bool bracket_check(string str) //checks, if numbers of opening and closing brackets are equal 
    {
        char c; //character, processing the string one character at a time
        StringStack stack;
        for (int i = 0; i < str.length(); i++)
        {
            c = str[i];
            if (c == '(')
            {
                stack.push("(");
            }
            else
            {
                if (c == ')')
                {
                    if (stack.empty())
                    {
                        return false;
                    }
                    else
                    {
                        while (stack.top() != "(")
                        {
                            stack.pop();
                        }
                        stack.pop();
                    }
                }
            }
        }
        if (!(stack.empty()))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    void prepare_string_for_postfix(string& str) //prepares string for calculation
    {
        str.erase(remove(str.begin(), str.end(), ' '), str.end());
        int i = 0;//index
        if ((str[i] == '-') || (str[i] == '.'))
        {
            str.insert(i, "0");
        }
        i = 1;
        while (i < str.length())
        {
            if (((str[i] == '-') || (str[i] == '.')) && (str[i - 1] == '('))
            {
                str.insert(i, "0");
            }
            i++;
        }
        i = str.length() - 1;
        if (str[i] == '=')
        {
            str.erase(i, 1);
        }
        str = str + "     ";
    }
    class StringStack //stack for forming postfix form and calculation
    {
    private:
        class elem
        {
        public:
            string data;
            elem* next;
            elem(string new_data)
            {
                data = new_data;
                next = NULL;
            }
        };
        elem* head;
    public:
        StringStack()
        {
            head = NULL;
        }
        void push(string data)
        {
            elem* tmp = new elem(data);
            elem* cur = head;
            head = tmp;
            head->next = cur;
        }
        void pop()
        {
            if (head == NULL)
            {
                throw out_of_range("No more elements");
            }
            else
            {
                elem* tmp = head;
                head = head->next;
                delete tmp;
            }
        }
        string top()
        {
            if (head == NULL)
            {
                throw out_of_range("Empty");
            }
            else
            {
                return head->data;
            }
        }
        bool empty()
        {
            return head == NULL;
        }
    };
public:
    postfix_string() //start of the new string in postfix form
    {
        head = NULL;
    }
    void push_back(string newData) //adds newData to the end of the string
    {
        if (empty())
        {
            insert_first(newData);
        }
        else
        {
            Node* tmp = new Node(newData);
            Node* cur = head; //current = head
            while (cur->next != NULL) { cur = cur->next; }
            cur->next=(tmp);
        }
    }
    bool empty() //checks if the postfix string are empty
    {
        return head == NULL;
    }
    void clear() //clears the entire string
    {
        Node* cur; //current
        while (head != NULL)
        {
            cur = head->next;
            delete head;
            head = cur;
        }
        head = NULL;
    }
    bool infixToPostfix(string& orig_string, string& bad_words) //checks if equation can be calculated,
    {                                                                                   // and transform equation into postfix form, if yes
        clear();//clears postfix form, prepares it for new equation
        bad_words.clear(); //cleans errors list
        prepare_string_for_postfix(orig_string); //do what the name of the function says
        if (bracket_check(orig_string) != true)
        {
            bad_words = "Unequal number of opening and closing brackets";
            return false; //the equation can't be calculated in this case
        }
        bool everything_ok = 1; //for knowing, if there any problem (errors)
        StringStack stack;
        string tmp; //temporal string for formation of postfix form
        int length = orig_string.length();
        int i = 0; //index for string
        char c; //character, processing the string one character at a time
        while (i < length)
        {
            tmp.clear();
            c = orig_string[i];
            switch (c)
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                while ((((c >= '0') && (c <= '9')) || (c == '.')) && (i < length))
                {
                    tmp = tmp + c;
                    i++;
                    c = orig_string[i];
                }
                push_back(tmp);
                break;
            case '(':
                tmp = c;
                stack.push(tmp);
                i++;
                break;
            case ')':
                while (stack.top() != "(")
                {
                    push_back(stack.top());
                    stack.pop();
                }
                stack.pop();
                i++;
                break;
            case 's':
                if ((orig_string[i + 1] == 'i') && (orig_string[i + 2] == 'n'))
                {
                    tmp = "sin";
                    i = i + 3;
                    while (!stack.empty() && prec(tmp) <= prec(stack.top())) {
                        push_back(stack.top());
                        stack.pop();
                    }
                    stack.push(tmp);
                }
                else
                {
                    if ((orig_string[i + 1] == 'q') && (orig_string[i + 2] == 'r') && (orig_string[i + 3] == 't'))
                    {
                        tmp = "sqrt";
                        i = i + 4;
                        while (!stack.empty() && prec(tmp) <= prec(stack.top())) {
                            push_back(stack.top());
                            stack.pop();
                        }
                        stack.push(tmp);
                    }
                    else
                    {
                        everything_ok = 0;
                        while (((!((c >= '0') && (c <= '9'))) && (!(c == '(')) && (!(c == ')'))) && (i < length))
                        {
                            bad_words += c;
                            i++;
                            c = orig_string[i];
                        }
                        bad_words += ' ';
                    }
                }
                break;
            case 'c':
                if ((orig_string[i + 1] == 'o') && (orig_string[i + 2] == 's'))
                {
                    tmp = "cos";
                    i = i + 3;
                    while (!stack.empty() && prec(tmp) <= prec(stack.top())) {
                       push_back(stack.top());
                        stack.pop();
                    }
                    stack.push(tmp);
                }
                else
                {
                    if ((orig_string[i + 1] == 't') && (orig_string[i + 2] == 'g'))
                    {
                        tmp = "ctg";
                        i = i + 3;
                        while (!stack.empty() && prec(tmp) <= prec(stack.top())) {
                            push_back(stack.top());
                            stack.pop();
                        }
                        stack.push(tmp);
                    }
                    else
                    {
                        everything_ok = 0;
                        while (((!((c >= '0') && (c <= '9'))) && (!(c == '(')) && (!(c == ')'))) && (i < length))
                        {
                            bad_words += c;
                            i++;
                            c = orig_string[i];
                        }
                        bad_words += ' ';
                    }
                }
                break;
            case 'l':
                if ((orig_string[i + 1] == 'n'))
                {
                    tmp = "ln";
                    i = i + 2;
                    while (!stack.empty() && prec(tmp) <= prec(stack.top())) {
                        push_back(stack.top());
                        stack.pop();
                    }
                    stack.push(tmp);
                }
                else
                {
                    if ((orig_string[i + 1] == 'o') && (orig_string[i + 2] == 'g'))
                    {
                        tmp = "log";
                        i = i + 3;
                        while (!stack.empty() && prec(tmp) <= prec(stack.top())) {
                            push_back(stack.top());
                            stack.pop();
                        }
                        stack.push(tmp);
                    }
                    else
                    {
                        everything_ok = 0;
                        while (((!((c >= '0') && (c <= '9'))) && (!(c == '(')) && (!(c == ')'))) && (i < length))
                        {
                            bad_words += c;
                            i++;
                            c = orig_string[i];
                        }
                        bad_words += ' ';
                    }
                }
                break;
            case 't':
                if ((orig_string[i + 1] == 'g'))
                {
                    tmp = "tg";
                    i = i + 2;
                    while (!stack.empty() && prec(tmp) <= prec(stack.top())) {
                        push_back(stack.top());
                        stack.pop();
                    }
                    stack.push(tmp);
                }
                else
                {
                    everything_ok = 0;
                    while (((!((c >= '0') && (c <= '9'))) && (!(c == '(')) && (!(c == ')'))) && (i < length))
                    {
                        bad_words += c;
                        i++;
                        c = orig_string[i];
                    }
                    bad_words += ' ';
                }
                break;
            case 'p':
                if ((orig_string[i + 1] == 'i'))
                {
                    tmp = "3.14159";
                    i = i + 2;
                    push_back(tmp);
                }
                else
                {
                    everything_ok = 0;
                    while (((!((c >= '0') && (c <= '9'))) && (!(c == '(')) && (!(c == ')'))) && (i < length))
                    {
                        bad_words += c;
                        i++;
                        c = orig_string[i];
                    }
                    bad_words += ' ';
                }
                break;
            case 'e':
                tmp = "2.71828";
                i = i + 1;
                push_back(tmp);
                break;
            case 'a':
                if ((orig_string[i + 1] == 'b') && (orig_string[i + 2] == 's'))
                {
                    tmp = "abs";
                    i = i + 3;
                    while (!stack.empty() && prec(tmp) <= prec(stack.top())) {
                        push_back(stack.top());
                        stack.pop();
                    }
                    stack.push(tmp);
                }
                else
                {
                    everything_ok = 0;
                    while (((!((c >= '0') && (c <= '9'))) && (!(c == '(')) && (!(c == ')'))) && (i < length))
                    {
                        bad_words += c;
                        i++;
                        c = orig_string[i];
                    }
                    bad_words += ' ';
                }
                break;
            case'+':
            case'-':
            case'*':
            case'/':
            case'^':
                tmp = c;
                i++;
                while (!stack.empty() && prec(tmp) <= prec(stack.top())) {
                    push_back(stack.top());
                    stack.pop();
                }
                stack.push(tmp);
                break;
            case' ':
                i++;
                break;
            default:
                everything_ok = 0;
                while (((!((c >= '0') && (c <= '9'))) && (!(c == '(')) && (!(c == ')'))) && (i < length))
                {
                    bad_words += c;
                    i++;
                    c = orig_string[i];
                }
                bad_words += ' ';
                break;
            }
        }
        while (!stack.empty()) {
            push_back(stack.top());
            stack.pop();
        }
        return everything_ok;
    }
    double get_result() //calculates the result of the equation
    {
        StringStack stack;
        double x = 0, y = 0;//variable for calculation
        Node* cur = head;//current=head
        while (cur != NULL)
        {
            if ((cur->data[0] >= '0') && (cur->data[0] <= '9'))
            {
                stack.push(cur->data);
                cur = cur->next;
            }
            else
            {
                if (cur->data == "cos")
                {
                    x = stod(stack.top());
                    stack.pop();
                    x = cos(x);
                    stack.push(to_string(x));
                    cur = cur->next;
                }
                else
                {
                    if (cur->data == "sin")
                    {
                        x = stod(stack.top());
                        stack.pop();
                        x = sin(x);
                        stack.push(to_string(x));
                        cur = cur->next;
                    }
                    else
                    {
                        if (cur->data == "tg")
                        {
                            x = stod(stack.top());
                            stack.pop();
                            x = tan(x);
                            stack.push(to_string(x));
                            cur = cur->next;
                        }
                        else
                        {
                            if (cur->data == "ctg")
                            {
                                x = stod(stack.top());
                                stack.pop();
                                x = 1/tan(x);
                                stack.push(to_string(x));
                                cur = cur->next;
                            }
                            else
                            {
                                if (cur->data == "ln")
                                {
                                    x = stod(stack.top());
                                    stack.pop();
                                    x = log(x);
                                    stack.push(to_string(x));
                                    cur = cur->next;
                                }
                                else
                                    if (cur->data == "log")
                                    {
                                        x = stod(stack.top());
                                        stack.pop();
                                        x = log10(x);
                                        stack.push(to_string(x));
                                        cur = cur->next;
                                    }
                                    else
                                    {
                                        if (cur->data == "sqrt")
                                        {
                                            x = stod(stack.top());
                                            stack.pop();
                                            x = sqrt(x);
                                            stack.push(to_string(x));
                                            cur = cur->next;
                                        }
                                        else
                                            if (cur->data == "abs")
                                            {
                                                x = stod(stack.top());
                                                stack.pop();
                                                x = fabs(x);
                                                stack.push(to_string(x));
                                                cur = cur->next;
                                            }
                                            else
                                            {
                                                if (cur->data == "+")
                                                {
                                                    x = stod(stack.top());
                                                    stack.pop();
                                                    y = stod(stack.top());
                                                    stack.pop();
                                                    x = x+y;
                                                    stack.push(to_string(x));
                                                    cur = cur->next;
                                                }
                                                else
                                                {
                                                    if (cur->data == "-")
                                                    {
                                                        x = stod(stack.top());
                                                        stack.pop();
                                                        y = stod(stack.top());
                                                        stack.pop();
                                                        x = y - x;
                                                        stack.push(to_string(x));
                                                        cur = cur->next;
                                                    }
                                                    else
                                                    {
                                                        if (cur->data == "*")
                                                        {
                                                            x = stod(stack.top());
                                                            stack.pop();
                                                            y = stod(stack.top());
                                                            stack.pop();
                                                            x = x * y;
                                                            stack.push(to_string(x));
                                                            cur = cur->next;
                                                        }
                                                        else
                                                        {
                                                            if (cur->data == "/")
                                                            {
                                                                x = stod(stack.top());
                                                                stack.pop();
                                                                y = stod(stack.top());
                                                                stack.pop();
                                                                x = y / x;
                                                                stack.push(to_string(x));
                                                                cur = cur->next;
                                                            }
                                                            else
                                                            {
                                                                x = stod(stack.top());
                                                                stack.pop();
                                                                y = stod(stack.top());
                                                                stack.pop();
                                                                x = pow(y, x);
                                                                stack.push(to_string(x));
                                                                cur = cur->next;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                    }
                            }
                        }
                    }
                }
            }
        }
        return stod(stack.top());
    }
    friend ostream& operator<<(ostream& stream, const postfix_string& str); //for showing postfix form
};
ostream& operator<<(ostream& stream, const postfix_string& str)
{
    if (str.head == NULL)
    {
        cout << "Nothing" << endl;
    }
    else
    {
        Node* node = str.head;
        while (node != NULL)
        {
            cout << node->data << " ";
            node = node->next;
        }
        cout << endl << endl;
    }
    return stream;
}

int main()
{
    bool we_calculate_it; //shows, can this equation be calculated, or errors occurred
    string equation; //equation, that should be calculated
    string for_errors; //message of occurred errors will be put here
    postfix_string postfix_form; //postfix form of the equation
    bool end=false; //for ending program
    while (!end)
    {
        cout << "\n\nAvailable operations:\n"
            << "+ , - , * , / , ^\n"
            << "cos, sin, tg, ctg, ln, log, sqrt, abs\n"
            << "Available constants:\n"
            << "pi, e\n\n"
            << "For enter print 'c' or 'close'\n\n\n";
        cout << "Enter you equation:" << endl;
        getline(cin, equation);
        if ((equation == "c") || (equation == "cloce"))
        {
            end = true;
        }
        else
        {
            we_calculate_it = postfix_form.infixToPostfix(equation, for_errors);
            if (we_calculate_it)
            {
                cout << "\nPostfix form:" << endl;
                cout << postfix_form;
                cout << "Result:\n" << postfix_form.get_result()<<"\n\n"<<endl;
            }
            else
            {
                if (for_errors == "Unequal number of opening and closing brackets")
                {
                    cout << "Can't calculate it, reason:" << endl;
                    cout << for_errors << endl;
                }
                else
                {
                    cout << "Can't calculate it, unknown operation:" << endl;
                    cout << for_errors << endl;
                }
            }
            cout << "\nDo you want to continue? (Y/N)\n"<<endl;
            getline(cin, equation);//same string is using here for memory economic porposes
            if ((equation == "N") || (equation == "n") || (equation == "NO") ||
                (equation == "No") || (equation == "no") || (equation == "nO"))
            {
                end = true;
            }
        }
    }
}