/*

 AUTHOR NAME :  HARSHIL RAMANI (HR)
 GITHUB : hr097
 FILE TYPE : C++
 REQUIREMENT : WINDOWS BASED 16/32/64 BIT OS SUPPORTED
 APPLICATION : FOLDER-HIDDEN-UNLOCKER

*/

#include <iostream>   //* C++ input output basic file
#include <windows.h>  //* Windows specific file for accessing Windows OS functions
#include <fstream>    //* C++ File hadling functions access
#include <stdlib.h>   //* standard Library Functions access
#include <unistd.h>   //* for sleep function(Timer) creating
#include <sys/stat.h> //* system OS property Access & dirExists()
#include <direct.h>   //* system OS property Access & dirExists()
#include <conio.h>    //* console input & output property access
#include <cwchar>    // * for console font chnage purpose


#ifndef _WIN32_WINNT  
  #define _WIN32_WINNT 0x0601
  #include<wincon.h>
  #include<string>

  //#endif  // user 1  e.g.DRASHTI //! EOP() seek.edit(*required) :DRASHTI DHOLA
  
  typedef struct _CONSOLE_FONT_INFOEX
  {
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT FontFamily;
    UINT FontWeight;
    WCHAR FaceName[LF_FACESIZE];
  }CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

  #ifdef __cplusplus
  extern "C"
  {
  #endif
    BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX  lpConsoleCurrentFontEx);
  #ifdef __cplusplus
  }
  #endif

#endif // user 2 e.g.HARSHIL //! EOP() seek.edit(*required) :HARSHIL RAMANI

using namespace std; // naming conflict resolving

//*****************************
 //? For Testing & Debuging 

void Debug(string t)  
{
  cout<<endl<<t<<getch();
}
void Debug(int t)  
{
  cout<<endl<<t<<getch();
}

//******************************/

class Unlocker
{
    private:

    string tempStorage,command,password,password_confirm;
    bool path_space_flag,r_flag,pass_flag;

    void setConsoleFont() //? for setting up console font
    {
      CONSOLE_FONT_INFOEX cfi;                                               // structure variable
      cfi.cbSize = sizeof(cfi);                                              // total bytes of cfi
      cfi.nFont = 0;                                                         //by default
      cfi.dwFontSize.X = 0;                                                  // Width of each character in the font
      cfi.dwFontSize.Y = 20;                                                 // Height fixed
      cfi.FontFamily = FF_DONTCARE;                                          // font family doesn't matter
      cfi.FontWeight = FW_NORMAL;                                            // font normally bold
      std::wcscpy(cfi.FaceName, L"JetBrains Mono Bold");                     // Choose your font BY SETTING FONT FACE
      SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi); // pass reference of that structure to OUTPUT HANDLE
    }

    void SetColor(int color) //?for setting individual text color
    {
      WORD wColor;                                      // color specifier
      HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); // create windows handle
      CONSOLE_SCREEN_BUFFER_INFO csbi;                  // We use csbi for the wAttributes word.
      
      if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
      {
        // Mask out all but the background attribute, and add in the forgournd color
        wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
        SetConsoleTextAttribute(hStdOut,wColor); // set console text color
      }
      
    }

    void scrClr(float i = 0.0) //?for clearing screen afer particular time
    {
      // by default set to zero with default arguements
      sleep(i);      //  going in sleep mode for particular time
      system("cls"); //  clear  the screen
    }

    void setCursorPos(int x, int y = 0) //? cursor position set ->IMPORTANT: relative postion is set
    {
  
      // vertical lines space
      while(x > 0)
      {
        cout << endl;
        x--;
      }
  
      // horizontal cursor space by 1 charecter
      while(y != 0)
      {
        printf(" ");
        y--;
      }
    }

    bool dirExists(const char *path) //checking function if directory exists or not 
    {
        struct stat info;
    
        if(stat(path,&info) != 0)
            return (false);
        else if(info.st_mode & S_IFDIR)
            return (true);
        else
            return (false);
    }

    bool fileExists(const string &path) //checking function if file exists or not 
    {   
       bool status_flag = false;
       ifstream read(path.c_str(),ios::binary|ios::in);     // file opened in binary mode 
       if(!read.is_open())
       {
        status_flag=false; // not exists 
       }
       else
       {
         status_flag=true; //exists
       }
       read.close();
       return(status_flag); 
    }

    bool checkSpInput(string &input)  //?function to check occurence of special charaters 
    {

        string list = "#%&{}*><$!:\'\"@+`|=";  
    
        int j=0,i=0;
        bool flag=false;
          
          for(i=0; i<list.length(); i++) //checking special  symbols
          {
               for(j=0;j<input.length();j++)
               { 
                 if(input[j] == ' ')
                 {   
                     path_space_flag=true;
                 }

                 if(input[1]==':') //first directory name label is valid  e.g. C:
                 continue;

                 if(input[j] == list[i])
                 {    
                      flag=true;
                      break;
                 }
                 else if(input[j] == '/'||input[0]=='/'||input[0]=='\\') //only / at first or last in invalid
                 { 
                      flag=true;
                      break;
                 }

               } 
          }
    
       return(flag);
    }

    void getDataFromFile(string path, string &input, int lineNo) //?get Data of given path file line in string
    {
          ifstream get(path.c_str(),ios::in); //file opened in reading mode 
      
          if(!get.is_open())//if file is not opened
          {  
              scrClr();
              SetColor(12);
              setCursorPos(2,30);
              cout << " ERROR : 404 ~Something went wrong !!!  ";
              scrClr(1.5);
              SetColor(15);
              exit(1);
          }
          else//if file is opened
          {
            int FileLine = 1; // start from first line
            while(FileLine<= lineNo)//whille fileline is <= lineno then
            {
              getline(get,input); //get data
              FileLine++;         //increment the fileline
            }
          }
         get.close(); //file closed
    }

    string getPath() // get path of folder that user want to lock
    {  
       repeat :

       tempStorage.clear();
       command.clear();
    
       scrClr();
       SetColor(10);
       setCursorPos(0,48);
       cout << " ~By Harshil Ramani  "; //author title
       SetColor(15);

       setCursorPos(2,2);
       cout<<"Enter full path of folder that you want to unlock : "; 
       SetColor(7);
       getline(cin,command);
       SetColor(15);
     
      if(checkSpInput(command))  //put validation 1 : check If path has any special charecter
      {
            setCursorPos(2,25);    
            SetColor(12);     
            cout << "Incorrect Path Format ~( Special Symobols aren't allowed ) !";
            scrClr(1.5);
            SetColor(15);
            goto repeat;
       
      }
      else
      {   
            if((!dirExists(command.c_str())) || command[command.length()-1] == '\\') // put validation 2 : check If directory Exists
            {    
                 setCursorPos(2,35);
                 SetColor(12);  
                 cout << " Incorrect Path / Folder not found !";
                 scrClr(1.5);
                 SetColor(15);
                 goto repeat;
            }
            else
            {     repeat_opt:
                 
                  if(r_flag) //repeat on invalid
                  {
                      tempStorage.clear();
        
                      scrClr();
                      SetColor(10);
                      setCursorPos(0,48);
                      cout << " ~By Harshil Ramani  ";
                      SetColor(15);
                             
                      setCursorPos(2,2);
                      cout<<"Enter full path of folder that you want to unlock : ";
                      SetColor(7);
                      cout<<command;
                      SetColor(15);

                      r_flag=false;
                  }
                 
                  setCursorPos(2,47);
                  SetColor(15);
                  cout << "Are You Sure ? Y/n : "; //asking for confirmation
                  SetColor(7);
                  getline(cin,tempStorage);
                  SetColor(15);
    
                  if(tempStorage=="Y"||tempStorage=="y") 
                  { 
                        if(fileExists(command+"\\pwd.txt"))
                        {
                            getDataFromFile(command+"\\pwd.txt",password_confirm,1);
                            pass_flag=true;
                        }
                        else
                        {
                            pass_flag=false;
                        }
                        return(command);
                  }
                  else if(tempStorage=="N"||tempStorage=="n") 
                  {
                      scrClr(0.5);
                      exit(0);
                  }
                  else
                  {   
                      setCursorPos(2,48);
                      SetColor(12);
                      cout << "Invalid Input !";
                      scrClr(1.5);
                      SetColor(15);
                      r_flag=true;
                      goto repeat_opt;
                  }

            }   
       }

      }


      string getpass(const char *prompt, bool show_asterisk=true) // ?password input as *
      {
          const char BACKSPACE=8;
          const char RETURN=13;
          SetColor(7);
        
          string pswd;
          unsigned char ch=0;
        
          cout <<prompt;
        
          DWORD con_mode;
          DWORD dwRead;
        
          HANDLE hIn=GetStdHandle(STD_INPUT_HANDLE);
        
          GetConsoleMode( hIn, &con_mode );
          SetConsoleMode( hIn, con_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT) );
        
          while(ReadConsoleA( hIn, &ch, 1, &dwRead, NULL) && ch !=RETURN)
            {
               if(ch==BACKSPACE)
                 {
                    if(pswd.length()!=0)
                      {  
                         if(show_asterisk)
                             cout <<"\b \b";
                         pswd.resize(pswd.length()-1);
                      }
                 }
               else
                 {
                     pswd+=ch;
                     if(show_asterisk)
                         cout <<'*';
                 }
            }
          cout <<endl;
          return pswd;
    }
   

    public:

    void unlock()
    {   
        string path = getPath();
        tempStorage = path;
        string temp_path;

        if(path_space_flag)
        { 
          temp_path.clear();
          temp_path = "\"";
          temp_path += tempStorage + "\"" ;
          tempStorage =temp_path;
        }
        temp_path.clear();
        temp_path = path + "\\locker.ini";

        if(!fileExists(temp_path.c_str())) // cross check if already locked
        {
          setCursorPos(2,45);
          SetColor(10);
          cout << " Folder is Already unlocked !";
          scrClr(1.5);
          SetColor(15);
          exit(0);
        }

        temp_path.clear();

        if(pass_flag) // check if user has password option
        { 
          short unsigned int attempt = 3;

          r_ask_psed:
         

          scrClr();
          setCursorPos(1,48);
          SetColor(15);
          cout<<" Remaining Attempts : ";
          SetColor(10);
          cout<<attempt;
          SetColor(15);

          setCursorPos(2,35);
          fflush(stdin);
          password =  getpass("Enter your password : ",true);
          SetColor(15);

          if(password_confirm!=password) // password verification
          {
              setCursorPos(2,48);
              SetColor(12);
              cout << "Incorrect Password ! ";
              scrClr(1.5);
              SetColor(15);
              attempt--;

              if(!attempt)
              {
                  setCursorPos(2,36);
                  SetColor(12);
                  cout << "Suspicious User Found-Access Denied !!! ";
                  scrClr(1.75);
                  SetColor(15);
                  exit(0);
              }
              else
              goto r_ask_psed;
          }
          else
          {  
              command.clear();

              temp_path.clear();
              temp_path = "del " ;
              temp_path += "\"" + path;
              temp_path += "\\pwd.txt";
              temp_path += "\"";

              
              system(temp_path.c_str());
              
              
              path = path + "\\locker.ini";
              
              command = "del ";
              command += "\"";
              command +=  path + "\"";

             
              system(command.c_str());
              command.clear();
              command = "attrib -h -s " + tempStorage;
              
              WinExec(command.c_str(),0);
                      
              setCursorPos(2,40);
              SetColor(10);
              cout << " Folder Unlocked Successfully ! "<<endl; // process success 
              scrClr(1.5);
              SetColor(15);
              exit(0);
              
          }
          
        }
        else
        {
             command.clear();

             path = path + "\\locker.ini";
             command = "del ";
             command =  command + path;
             system(command.c_str());
             command.clear();
             command = "attrib -h -s " + tempStorage;
             WinExec(command.c_str(),0);
                     
             setCursorPos(2,40);
             SetColor(10);
             cout << " Folder Unlocked Successfully !"<<endl; // process success 
             scrClr(1.5);
             SetColor(10);
             exit(0);
        }


    }

    Unlocker()
    {
      path_space_flag=r_flag=pass_flag=false;
      setConsoleFont();
    }
    ~Unlocker()
    {
   
    }

    protected:

};

int main()
{   
    Unlocker U;
    U.unlock();
    return(0);
}