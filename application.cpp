/*
    UNIVERSITY MANAGEMENT SYSTEM
    	MADE BY : GROUP II
    UNIVERSITY : ROYAL UNIVERSITY OF PHNOM PENH
*/

/**
 * Author: Bunchhean Chhoung
 **/ 

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <cstring>
#include <conio.h>
#include <cstdio>

using namespace std;

#define RE  "\033[0m"   
#define R   "\033[31m"  
#define G   "\033[32m"
#define B  "\033[1m\033[33m"      

#define MAX 50

#define TEACHER_FILE "teacher.bin"
#define ENROLLMENT_FILE "enrollment.bin"
#define STUDENT_FILE "student.bin"
/*
	[+] Core:
	--------------------------------------------------------------
	05. Admin : manage users(admins, logins, security).	
	04. Calculate : record marks, calculator GPA/Grade.
	03. Enrollment : create course enroll student;
	02. Uni Management : manage teacher staff;
	01. Student Management : register student store personal data.
*/
void pause() { // press key to continue
	cout << "\n\tPress any key to continue...";
	getch();
}

void clear() { // Clear Screen Console
	system("cls||clear");
}

struct Report {
	int totalOfStudent;
	int totalOfDropStudent;
};

void Calculate(){
    float markObtained;
    char grade;
    float GPA;

    if(markObtained == 100 || markObtained >= 90) {
        grade = 'A';
        GPA = 4.0;
        }else if (markObtained == 90 || markObtained >= 85) {
            grade = 'B+';
            GPA = 3.5;
        }else if (markObtained == 85 || markObtained >= 80) {
            grade = 'B'; 
            GPA = 3.0;
        }else if (markObtained == 80 || markObtained >= 75) {
            grade = 'C+'; 
            GPA = 2.5;
        }else if (markObtained == 75 || markObtained >= 70) {
            grade = 'C'; 
            GPA = 2.0;
        }else if (markObtained == 70 || markObtained >= 65) {
            grade = 'D+';
            GPA = 1.5;
        }else if (markObtained == 65 || markObtained >= 60) {
            grade = 'D+'; 
            GPA = 1.0;
        }else {
            grade = 'F'; 
            GPA = 0.0;
        }
};

struct AuthData {	
    char name[MAX];
    char contact[MAX];
    char password[MAX];
}ad;

class Admin : public AuthData {
public: 
	void save(ofstream& of){
		of.write(ad.name,sizeof(name));
		of.write(ad.contact,sizeof(contact));
		of.write(ad.password,sizeof(password));
	}
	void load(ifstream& inf){
		inf.read(ad.name,sizeof(name));
		inf.read(ad.contact,sizeof(contact));
		inf.read(ad.password,sizeof(password));
	}
public:
    void showdata();
	void signUp();   
    void login();
    void logout();
    void changepassword();
	void forgetPassword();
    void verify();
	void AddAdmin();
	void AdminList();
	void deleteAdmin();	
}users;

void Admin::signUp() { 
	ofstream writeFile("Admin.bin",ios::out|ios::binary|ios::app);
	if(!writeFile.is_open()){
		cerr << "\n\n\tError file can't opening...";
		return;
	}

	cout << "\n\n\tENTER USER NAME: ";
	cin.seekg(0); 
	cin.getline(name,MAX);
	
	cout << "\n\tENTER USER EMAIL/PHONE: ";
	cin.seekg(0);
	cin.getline(contact,MAX);	
	
	cout << "\n\tCREATE USER PASSWORD: ";
    int m = 0;
    char ch;
    while (true) {
        ch = getch();
        if (ch == '\r') break;
        if (ch == '\b') {
            if (m > 0) {
                m--;
                cout << "\b \b";
            }
        } else if (m < MAX - 1) {
            password[m++] = ch;
            cout << "#";
        }
    }
    password[m] = '\0';
    
//	users.save(writeFile);
	writeFile.write((char*)name,strlen(name)+1);
	writeFile.write((char*)contact,strlen(contact)+1);
	writeFile.write((char*)password,strlen(password)+1);
	writeFile.close();
	
	cout << "\n\n\tYour Account Name " << name << " created Successfull...";
	pause();
}
void Admin::login() {
    char ch;
    int attempts = 3;
    bool found = false;
    int i = 0;
    char searchName[MAX],searchPassword[MAX];
    clear();
    ifstream readFile("Admin.bin", ios::in | ios::binary);
    if (!readFile.is_open()) {
        cout << R << "\n\n\tNo Admin file found! Please Sign Up first." << RE;
        signUp();
        return;
    }
    
	users.load(readFile);
	cout << B << "\n\n\n\t[+] SENTION LOGIN: " << RE;
    for (int i = 0; i < attempts; i++) {
        cout << "\n\n\tENTER USER NAME: ";
        cin.seekg(0);
        cin.getline(searchName, MAX);

        cout << "\n\tENTER USER PASSWORD: ";
        int m = 0;
        while (true) {
            ch = getch();
            if (ch == '\r') break; 
            else if (ch == '\b') { 
                if (m > 0) {
                    m--;
                    cout << "\b \b";
                }
            } else if (m < MAX - 1) {
                searchPassword[m++] = ch;
                cout << '*';
            }
        }
        searchPassword[m] = '\0';

        // Reset file to beginning for each login attempt
        // Go back to start of file for each try
        readFile.clear();
        readFile.seekg(0, ios::beg);

        // Check every record in file
        while (readFile.read((char*)&ad, sizeof(ad))) {
            if (strcmp(name, searchName) == 0 && strcmp(password, searchPassword) == 0) {
                cout << G << "\n\n\tLogin Successful! Welcome " << searchName << "." << RE;
                found = true;
                return;
            }
        }

        if (!found) {
            int left = attempts - i - 1;
            if (left > 0) {
                cout << R << "\n\n\tInvalid username or password. " << left << " attempts left." << RE;
                getch();
            } else {
                cout << R << "\n\n\tToo many failed attempts! Program is exiting..." << RE;
                exit(0);
            }
        }
	}    
    readFile.close();
}
void Admin::logout() { // Logout Admin
	cout << G << "\n\n\tYour Account Has Logout Sucesss..." << RE;
	exit(0);
}
void Admin::changepassword() { // Change Password Admin
	char ch;
	char newPassword[MAX];
 	int m = 0, incorrect = 0;
    ifstream readFile;
    ofstream writeFile;

    readFile.open("Admin.bin", ios::in | ios::binary);
    if (!readFile.is_open()) {
        cout << R << "\n\t\tNo existing password found!\n" << RE;
        return;
    }

	readFile.read(newPassword,sizeof(newPassword));
    cout << "\n\n\tENTER YOUR NEW PASSWORD: ";
    while (true) {
        ch = getch();
        if (ch == '\r') break; 
        else if (ch == '\b') { 
            if (m > 0) {
                m--;
                cout << "\b \b";
            }
        } else if (m < MAX - 1) {
            newPassword[m++] = ch;
            cout << '*';
            }
        }
    newPassword[m] = '\0';

    readFile.read(ad.password,sizeof(password));
    while (incorrect < 3) {
    cout << "\n\n\tENETR YOUR OLD PASSWORD: ";
    while (true) {
        ch = getch();
        if (ch == '\r') break; 
            else if (ch == '\b') { 
                if (m > 0) {
                    m--;
                    cout << "\b \b";
                }
            } else if (m < MAX - 1) {
                password[m++] = ch;
                cout << '*';
            }
        }
        password[m] = '\0';
        
		while(readFile.read((char*)&ad,sizeof(ad))){
	        if (strcmp(ad.password, newPassword) == 0) {
	            readFile.close();
	            writeFile.open("Admin.bin", ios::binary|ios::out);
//	            writeFile.write((char*)newPassword, strlen(newPassword) + 1);
				writeFile.write(newPassword,sizeof(newPassword));
	            writeFile.close();
	            cout << endl << G << "\n\t\tyour password has been changed successfully! \n" << RE;
	            return;
	        } else {
	            if (incorrect == 2) {
	                cout << endl << R << "\n\tyou have entered the wrong password 3 times. Program is Exiting...\n" << RE;
	                readFile.close();
	                exit(0);
	            }
	            cout << endl << R << "\n\tIncorrect password, Try again!\n" << RE;
	            ++incorrect;
	        }
	    }
    }
    readFile.close();
    writeFile.close();
    pause();
}
void Admin::forgetPassword() { // Forget Password Admin
	char ch;
	int attempts = 3;
	char searchName[MAX],searchEmail[MAX];
	bool found = false;
	clear();
	
	ifstream readFile("Admin.bin",ios::in|ios::app|ios::binary);
	if(!readFile.is_open()){
		cerr << "\n\n\tError file can't opening...";
		return;	
	}	
	
	users.load(readFile);
	cout << B << "\n\n\n\t[+] SECTION FORGET PASSWORD: " << RE;	
	for ( int i = 0; i < attempts ; i++ ){
		cout << "\n\n\tENTER USER NAME: ";
		cin.seekg(0);
		cin.getline(searchName, MAX);
		
		cout << "\n\tENTER USER EMAIL/PHONE: ";
		cin.seekg(0);
		cin.getline(searchEmail,MAX);
		
		// Reset file to beginning for each login attempt
        // Go back to start of file for each try
        readFile.clear();
        readFile.seekg(0, ios::beg);
		
		while(readFile.read((char*)&ad, sizeof(ad))){
			if(strcmp(name,searchName) == 0){
				if(strcmp(contact,searchEmail) == 0){
					cout << G <<"\n\n\tLVERIFY SUCCESSFULL...";
					cout << G <<"\n\n\tUSER PASSWORD: " << password << RE;
					getch();
					users.login();
					return;
				}else {
					int left = attempts - i - 1;
					if(left>0){
						cout << R << "\n\n\tWrong Information. You have " << left << " attempts left." << RE;
					}else {
						cout << "\n\n\tProgram is exiting..!";
						exit(0);
					}
				}
			} 
		}                           
	}
	readFile.close();	
}
void Admin::verify() { // Verify Admin
	char ch;
	int attempts = 3;
	int i = 0;
	char searchPassword[MAX],searchName[MAX];
	
	ifstream readFile("Admin.bin",ios::in|ios::binary);
	if(!readFile.is_open()){
		cerr << "\n\n\tError file can't opening...";
		return;	
	}	

	for ( int i = 0; i < attempts ; i++ ){
		cout << "\n\tENTER USER PASSWORD: ";
		int m = 0;
		while (true) {
			ch = getch();
			if (ch == '\r') break;                 
			else if (ch == '\b') {                 
				if (m > 0) {
					m--;
					cout << "\b \b";               
				}
			}
			else if (m < MAX - 1) {                
				searchPassword[m++] = ch;
				cout << '#';
			}
		}
		searchPassword[m] = '\0'; 
		
		if(strcmp(password,searchPassword) == 0){	
			cout << "\n\n\tLOGIN SUCCESSFULL...";	
			cout << "\n\tUSER NAME: " << searchName;		
			getch();			
			return;			
		}else {				
			int left = attempts - i - 1;				
			if(left>0){
				cout << R << "\n\tWrong Password. You have " << left << " attempts left." << RE;
			}else {
				cout << "\n\n\tProgram is exiting..!";
				exit(0);
			}
		}                            
	}
	readFile.close();	
}
void Admin::AddAdmin() { // Add New Admin 
	char ch;
	
	ofstream writeFile("Admin.bin",ios::binary|ios::out);
	if(!writeFile.is_open()){
		cerr << "\n\n\tError file can't opening...";
		return;			
	}
	
	cout << "\n\n\n\tCREATE NEW ACCOUNT: ";
	
	cout << "\n\n\t[+] USER NAME: ";
	cin.seekg(0);
	cin.getline(name,sizeof(name));
	
	cout << "\n\t[+] USER EMAIL/PHONE: ";
	cin.seekg(0);
	cin.getline(contact,sizeof(contact));
	
	cout << "\n\t[+]USER PASSWORD: ";
		int m = 0;
		while (true) {
			ch = getch();
			if (ch == '\r') break;                
			else if (ch == '\b') {                 
				if (m > 0) {
					m--;		
					cout << "\b \b";               
				}
			}
			else if (m < MAX - 1) {                
				password[m++] = ch;
				cout << '#';
			}
		}
		password[m] = '\0'; 
		
	users.save(writeFile);
	writeFile.close();
	pause();
}
void AdminField(){ // Admin field
	cout << "\n\n\n";
	cout << "\t"<< string(67,'-');
	cout << left 
		 << "\n\t" << setw(22)
		 << "|Name" << setw(22)
		 << "|Email" << setw(22)
		 << "|Password" << '|' 
		 << endl;
	cout << "\t"<< string(67,'-');
}
string MarkHidden(char* password) {
    string hidden = "";
    for (int i = 0; password[i] != '\0'; i++) {
        hidden += '#'; 
    }
    return hidden;
}
void Admin::showdata() {
    cout << left
         << "\n\t| " << setw(20) << name
         << "| " << setw(20) << contact
         << "| " << setw(20) << MarkHidden(password)
         << "|"
         << endl;
}
void Admin::AdminList() {
	bool found = false;
	
	ifstream readFile("Admin.bin",ios::binary);
	if(!readFile.is_open()){
		cerr << "\n\n\tError file can't opening...";
		return;
	}
	
	AdminField();
	
	while(readFile.read((char*)&ad,sizeof(ad))){
		showdata();
		found = true;
	}
	
	cout << "\t" << string(67,'-') << endl;
	
	readFile.close();
	
	if(!found){
		cout << "\n\tRecord Not exist..." << endl;
	}

	pause();
}
void Admin::deleteAdmin() {
	bool found = false;
 	char searchName[MAX],searchPassword[MAX];
 	
	ifstream readFile("Admin.bin",ios::in|ios::binary);
	ofstream tempfile("tempAdmin.bin", ios::out|ios::binary);
	
	if(!readFile||!tempfile){
		cerr << "\n\n\tErorr file can't Opeing..";
		return;
	}
	
	cout << "\n\n\tENTER USER NAME: ";
	cin.seekg(0);
	cin.getline(searchName,MAX);
	
	cout << "\n\n\tENTER USER PASSWORD: ";
	cin.seekg(0);
	cin.getline(searchPassword,MAX);
	
	while(readFile.read((char*)&ad,sizeof(ad))){
		if(strcmp(name,searchName) == 0 && strcmp(password,searchPassword) == 0){
			cout << "\n\n\tDeleted Admin Name: " << searchName << endl;
			found = false;
		}
		tempfile.write((char*)&ad,sizeof(ad));
	}
	
	readFile.close();
	tempfile.close();

	rename("Admin.bin", "Admin_backup.bin");
	rename("tempAdmin.bin","Admin.bin"); 

	if(found){
		cout << "\n\n\tRecord Admin Name " << searchName << " has been Deleted Successfull...";
	}else {
		cout << "\n\n\tRecord Not Found!";
	}
}

class University{
public:
	int id;
	char name[MAX];
	char contact[MAX];
	char sex;
	char specailization[MAX];
public:    
	void getdata();
	void addTeacher();
	void updateTeacher();
	void DeleteTeacher();
	void displayTeacher();
	int getID() { return id; }
}uni;

void University::getdata(){ // use in add teacher
	cout << "\n\tEnter Teacher/Teacher/Professor ID: ";
	cin >> id;

	cout << "\n\tEnter Instructor/Teacher/Professor Name: ";
	cin.seekg(0);
	cin.getline(name,MAX);

	cout << "\n\tEnter Instructor/Teacher/Professor Sex: ";
	cin >> sex;

	cout << "\n\tEnter Specailization: ";
	cin.seekg(0);
	cin.getline(specailization, MAX);
}
void University::addTeacher() {  // add teacher to file teacher.bin

	ofstream writeFile(TEACHER_FILE, ios::binary | ios::out | ios::app);
	if (!writeFile.is_open()) {
		cerr << "\n\n\tError Opening file for writing!";
		return;
	}

	getdata();
	writeFile.write((char*)&uni, sizeof(uni));
	writeFile.close();
	pause();

}
void fieldOfTeacher() { // it's just a field of data
	cout << endl << string(75,'-') << endl;
    cout << left
		 << setw(10) << "|ID"
		 << setw(20) << "|Name"	
		 << setw(5)  << "|Sex"	
		 << setw(20) << "|Contact"	
		 << setw(20) << "|Specailize 		       |"
		 << endl;
	cout << endl << string(75,'-') << endl;
}
void University::displayTeacher() { // Display data information
	bool success = false;
	ifstream readFile(TEACHER_FILE, ios::binary);
	if(!readFile.is_open()) {
		cerr << "\n\n\tError Opening file for reading!";
		return;
	}
	fieldOfTeacher();
	while(readFile.read((char*)&uni, sizeof(uni))) {
		cout << left << '|'
			 << setw(10) << id << '|'
			 << setw(20) << name << '|'
			 << setw(5)  << sex << '|'
			 << setw(20) << contact << '|'
			 << setw(20) << specailization << '|'
			 << endl;
		success = true;
	}

	cout << string(75,'-') << endl;
	readFile.close();

	if (!success) {
		cout << "\n\tRecord not exist.." << endl;
	}
	pause();
}
void University::DeleteTeacher() { // Delete data from record
	int idtodelet;
	ifstream readFile;
	readFile.open(TEACHER_FILE, ios::binary);
	if(!readFile) {
        cerr << "\n\n\tError opening file! ";
        return;	
	}	
	ofstream writeFile;
	writeFile.open("Temp.bin", ios::out);
	readFile.seekg(0, ios::beg);
	cout << "\n\tEnter Instructor/Teacher/Professor ID to Update: ";
	cin >> idtodelet;
	while(readFile.read((char*)&uni,sizeof(uni))){
		if(uni.getID() != idtodelet){
			writeFile.write((char*)&uni,sizeof(uni));
		}
	}
	writeFile.close();
	readFile.close();
	rename(TEACHER_FILE, "teacher_backup.bin");
	rename("Temp.bin",TEACHER_FILE);
	cout << "\n\n\tRecord Deleted..." << endl;
	pause();
}
void University::updateTeacher() { // Update date from record
	bool success = false;
	int idToUpdate;
	fstream file;
	file.open(TEACHER_FILE, ios::binary|ios::in|ios::out);
	if(!file){
        cerr << "\n\n\tError opening file! ";
        return;	
	}
	cout << "\n\tEnter Instructor/Teacher/Professor ID to Update: ";
	cin >> idToUpdate;
	while(!file.eof() && success == false){
		file.read((char*)&uni,sizeof(uni));
		if(uni.getID() == idToUpdate){
			uni.displayTeacher();
			cout << "\n\n\tPlease Enter The New Details of Student.." << endl;
			uni.getdata();
			int pos=(-1)*static_cast<int>(sizeof(uni));
			file.seekp(pos,ios::cur);
			file.write((char*)&uni, sizeof(uni));
			cout << "\n\n\tRecord Updated.."<<endl;
			success = true;
		}
	}
	file.close();
	if(!success){
		cout << "\n\n\tRecord Not Found!" << endl;
	}
	pause();
}

/* STUDENT MANAGE PERSONAL INFORMATION */

class Student {
private:
    int id;
    char name[MAX];
    char gender[5];
    char contact[MAX];
    char address[MAX];
    char phone[MAX];
    char faculty[MAX];
    char major[5];
    char yearLevel;
public:
    void addStudent();
    void displayStudent();
    void registerStudent();
    void viewStudent();
    void updateStudent();
    void deleteStudent();
    void searchStudents();
    int  getID() { return id; }
}stu;

void fieldOfStudent() {
    cout << endl;
    cout << string(127, '-') << endl;
    cout << left
         << setw(10) << "|ID"
         << setw(20) << "|Name"
         << setw(5)  << "|Sex   "
         << setw(20) << "|Email"
         << setw(20) << "|Address"
         << setw(15) << "|Phone"
         << setw(20) << "|Faculty "
         << setw(8)  << "|Major"
         << setw(8)  << "|Year |" << endl;
    cout << string(127, '-') << endl;
}
void Student::addStudent() {

    cout << "\n\n\tEnter ID: "; 
	cin >> id;
	cin.ignore();

    cout << "\n\tEnter Full Name: "; 
	cin.seekg(0);
	cin.getline(name, MAX);

    cout << "\n\tEnter Gender(M/F): "; 
	cin >> gender;

    cout << "\n\tEnter Contact Email: "; 
	cin.seekg(0);
	cin.getline(contact, MAX);

    cout << "\n\tEnter current address: "; 
    cin.seekg(0);
	cin.getline(address, MAX);

    cout << "\n\tEnter Phone number: "; 
    cin.seekg(0);
	cin.getline(phone, MAX);

    cout << "\n\tEnter Faculty: "; 
    cin.seekg(0);
	cin.getline(faculty, MAX);

    cout << "\n\tEnter Department/major: "; 
    cin.seekg(0);
	cin.getline(major, 5);

    cout << "\n\tEnter year(1/2/3/4): "; 
	cin >> yearLevel;
}
void Student::displayStudent() {
    cout << left << '|'
         << setw(9)  << id << '|'
         << setw(19) << name << '|'
         << setw(6)  << gender << '|'
         << setw(19) << contact << '|'
         << setw(19) << address << '|'
         << setw(14) << phone << '|'
         << setw(19) << faculty << '|'
         << setw(7)  << major << '|'
         << setw(5)  << yearLevel << '|' <<endl;
}
void Student::registerStudent() {
    int i = 0;
    ofstream writeFile(STUDENT_FILE, ios::binary | ios::app | ios::out);

    if (!writeFile.is_open()) {
        cerr << "\n\n\tError opening file for writing!";
        return;
    }

    addStudent();
    writeFile.write((char*)&stu, sizeof(stu));

    writeFile.close();
    cout << G << "\n\n\tData has been stored in students.bin file successfully!" << RE;
    cout << "\n\tPress any key to return to Students Menu...";
    getch();
}
void Student::viewStudent() {
	bool success = false;
    int i = 0;

    ifstream readFile(STUDENT_FILE, ios::binary);
    if (!readFile.is_open()) {
        cerr << "\n\n\tError opening file for reading or no file exists!";
        readFile.close();
        return;
    }

    fieldOfStudent();
    while (readFile.read((char*)&stu,sizeof(stu))) {
        stu.displayStudent();
        success = true;
    }

    cout << string(127,'-') << endl;
    readFile.close();

    if (!success) {
        cout << "\n\n\n\tRecord not exist!" << endl;
    }

    cout << "\n\tPress any key to continue...";
    getch();
}
void Student::deleteStudent() {
	bool success = false;
    int idToDelete, i = 0;
    ifstream readFile(STUDENT_FILE, ios::in | ios::binary);
    ofstream tempFile("temp.bin", ios::out | ios::binary);

    cout << "\n\n\tEnter student to id delete: ";
    cin >> idToDelete;

    if (!readFile) {
        cerr << "\n\n\tError opening file! ";
        return;
    }

    if (!tempFile) {
        cerr << "\n\n\tError opening file! ";
        return;
    }

    while(readFile.read((char*)&stu, sizeof(stu))) {
        if (stu.getID() == idToDelete) {
            cout << "\n\tDeleting Student: " << idToDelete << endl;
            success = true;
            continue;
        }
        tempFile.write((char*)&stu, sizeof(stu));
    }

    readFile.close();
    tempFile.close();

    remove(STUDENT_FILE);
    rename("temp.bin", STUDENT_FILE);

    if (success) {
        cout << "Record with ID " << idToDelete << " deleted successfully.\n";
    }else {
        cout << "\n\tRecord not Found.";
    }

}
void Student::updateStudent() {
	int oldID, newID, newYear, recordIndex = 0;
    char option;
    char newName[MAX], 
         newEmail[MAX], 
         newPhone[MAX];
    bool found = false;
    fstream readFile; // user fstream it can be both read and write

    cout << "\n\n\tUPDATE INFORMAITON: ";
    cout << "\n\n\t>Enter (i/I) to Update ID.\n"
         << "\n\t>Enter (n/N) to Update Name.\n"
         << "\n\t>Enter (e/E) to Update Email.\n"
         << "\n\t>Enter (p/P) to Update Phone.\n";
    cout << "\n\tChoice option: ";
    option = getche(); // get 1 charactor from keybaord

    readFile.open(STUDENT_FILE, ios::in | ios::out |ios::binary);

    if (!readFile) {
        cerr << "\n\n\tFile not Found!";
        return;
    }
    
    switch(option) {

        case 'i':
        case 'I': {

            cout << "\n\t>Enter Current ID to Update: ";
            cin >> oldID;
            cout << "\n\t>Enter new ID: ";
            cin >> newID;

            while (readFile.read((char*)&stu, sizeof(stu))) {
                if (stu.id == oldID) {
                    stu.id = newID;
                    readFile.seekp(recordIndex * sizeof(stu), ios::beg);
                    readFile.write((char*)&stu, sizeof(stu));
                    cout << G << "\n\tID change success."<< RE;
                    found = true;
                    break;
                }
                recordIndex++;
            }
            
            if (!found) {
                cout << R << "\n\tID not Found!" << RE;
                break;
            }
            break;
        }

        case 'n':
        case 'N':{

            cout << "\n\n\t>Enter Current ID to Update: ";
            cin >> oldID;
            cin.ignore();
            cout << "\n\t>Enter new Name: ";
            cin.getline(newName,MAX);
            
            while (readFile.read((char*)&stu, sizeof(stu))) {
                if (stu.id == oldID) {
                    strcpy(stu.name, newName);
                    readFile.seekp(recordIndex * sizeof(stu), ios::beg);
                    readFile.write((char*)&stu, sizeof(stu));
                    cout << G << "\n\tName change success."<< RE;
                    found = true;
                    break;
                }
                recordIndex++;
            }
            
            if (!found) {
                cout << R << "\n\tName not Found!" << RE;
                break;
            } 
            break;
        }

        case 'e':
        case 'E': {

            cout << "\n\n\t>Enter Current ID to Update: ";
            cin >> oldID;
            cin.ignore();
            cout << "\n\t>Enter new Email: ";
            cin.getline(newEmail,MAX);
            
            while (readFile.read((char*)&stu, sizeof(stu))) {
                if (stu.id == oldID) {
                    strcpy(stu.contact, newEmail);
                    readFile.seekp(recordIndex * sizeof(stu), ios::beg);
                    readFile.write((char*)&stu, sizeof(stu));
                    cout << G << "\n\tEmail change success."<< RE;
                    found = true;
                    break;
                }
                recordIndex++;
            }
            
            if (!found) {
                cout << R << "\n\tEmail not Found!" << RE;
                break;
            } 
            break;   
        }

        // Update Phone number
        case 'p':
        case 'P': {

            cout << "\n\n\t>Enter Current ID to Update: ";
            cin >> oldID;
            cin.ignore();
            cout << "\n\t>Enter new Phone number: ";
            cin.getline(newPhone,MAX);
            
            while (readFile.read((char*)&stu, sizeof(stu))) {
                if (stu.id == oldID) {
                    strcpy(stu.phone, newPhone);
                    readFile.seekp(recordIndex * sizeof(stu), ios::beg);
                    readFile.write((char*)&stu, sizeof(stu));
                    cout << G << "\n\tPhonenumber change success."<< RE;
                    found = true;
                    break;
                }
                recordIndex++;
            }
            
            if (!found) {
                cout << R << "\n\tPhonenumber not Found!" << RE;
                break;
            } 
            break;
        }
        readFile.close();
    }
    cout << "\n\tPress any key to continue...";
    getch();
}
void Student::searchStudents() {
    int i = 0;
    int searchID;
    bool found = false;
    ifstream readFile;

    cout << "\n\n\tEnter Student ID to Search: ";
    cin >> searchID;

    readFile.open(STUDENT_FILE, ios::in | ios::binary);

    if (!readFile.is_open()) {
        cerr << R <<"\n\t\t\tError opening file!" << RE;
        return;
    }

    while(readFile.read((char*)&stu, sizeof(stu))) {
        if(stu.getID() == searchID) {
            fieldOfStudent();
            stu.displayStudent();
            cout << string(127, '-') << endl;            
            found = true;
            break;
        }
        i++;
    }
    if(!found) {
        cerr << R << "\n\tID is not found!" << RE;
    }
    readFile.close();
    cout << "\n\tPress any key to continue...";
    getch();
}

/* ENROLLMENT SECTION ASSIGN STUDETN + COURSE */

class Enrollment {
private:
    char studentName[MAX], courseID[MAX], courseName[MAX];
    char semester[MAX], status[MAX];
    int creditsHours;
    int enrollID, studentID;
public:
    void enrollStudent();
    void dropStudent();
    void displayAll();
    int getId(){ return enrollID; }
}e;

void Enrollment::enrollStudent() {
    cout << "\n\n\tEnter Enroll ID: ";
    cin >> enrollID;

    cout << "\n\tEnter Student ID: ";
    cin >> studentID;
    cin.ignore();

    cout << "\n\tEnter Student Name: ";
    cin.seekg(0);
    cin.getline(studentName, MAX);

    cout << "\n\tEnter Course ID: ";
    cin.seekg(0);
    cin.getline(courseID, MAX);

    cout << "\n\tEnter Course Name: ";
    cin.seekg(0);
    cin.getline(courseName, MAX);

    cout << "\n\tEnter Credits: ";
    cin >> creditsHours;
    cin.ignore(); 

    cout << "\n\tEnter Semester (I/II): ";
    cin.seekg(0);
    cin.getline(semester, MAX);

    cout << "\n\tEnter Status: ";
    cin.seekg(0);
    cin.getline(status, MAX);

    ofstream writeFile(ENROLLMENT_FILE, ios::binary | ios::app);
    if (!writeFile.is_open()) {
        cerr << "\n\n\tError opening enrollment file for writing!";
        return;
    }

    writeFile.write((char*)&e, sizeof(e));
    writeFile.close();
	pause();

}
void Enrollment::dropStudent() {
	bool success = false;
    int idTodelete;
    int i  = 0;

    cout << "\n\n\tEnter Enroll ID to drop a Student: ";
    cin >> idTodelete;

    ifstream readFile(ENROLLMENT_FILE, ios::in | ios::binary);
    ofstream tempFile("temp_enroll.bin", ios::out | ios::binary);

    if (!readFile.is_open() || !tempFile.is_open()) {
        cerr << "\n\n\tError file can't opening...";
        return;
    }

    while (readFile.read((char*)&e, sizeof(e))) {
        if (e.getId() == idTodelete) {
            cout << G << "\n\tDropped Enroll ID " << idTodelete
                 << " - Student removed from Course." << RE;
            success = true;
            continue; 
        }
        tempFile.write((char*)&e, sizeof(e));
    }

    readFile.close();
    tempFile.close();

    if (success) {
        remove(ENROLLMENT_FILE);
        rename("temp_enroll.bin", ENROLLMENT_FILE);
        cout << "\n\tRecord with ID " << idTodelete << " deleted successfully.\n";
    } else {
        remove("temp_enroll.bin"); 
        cout << "\n\tRecord not Found.";
    }
	pause();
}
void Enrollment::displayAll() {
	int i = 0;
    ifstream file(ENROLLMENT_FILE, ios::binary);
    if (!file) {
        cerr << "\n\n\tNo enrollment records found!";
        return;
    }
    cout << endl << string(123, '-') << endl;
    cout << left
         << setw(10) << "|EnrollID"
         << setw(12) << "|StudID"
         << setw(20) << "|StudentName"
         << setw(12) << "|CourseID"
         << setw(30) << "|CourseName"
         << setw(15) << "|Credits"
         << setw(10) << "|Semester"
         << setw(12) << "|Status      |" << endl;
    cout << string(123, '-') << endl;

    Enrollment e;
    while (file.read((char*)&e, sizeof(e))) {
        cout << left << '|'
             << setw(9) << e.enrollID << '|'
             << setw(11) << e.studentID << '|'
             << setw(19) << e.studentName << '|'
             << setw(11) << e.courseID << '|'
             << setw(29) << e.courseName << '|'
             << setw(14) << e.creditsHours << '|'
             << setw(9) << e.semester << '|'
             << setw(12) << e.status << '|' 
             << endl;
    }
    cout << string(123, '-') << endl;

    file.close();
    cout << "\n\tPress any key to continue...";
    getch();
}

void intro() {
    clear();
    cout << B << "\n\n\n\n\n\tUNIVERSITY MANAGEMENT SYSTEM" << RE;
    cout << B << "\n\n\tMADE BY : GROUP II" << RE;
    cout << B <<"\n\n\tUNIVERISTY : ROYAL UNIVERSITY OF PHNOM PENH" << RE;
    getch();
}
void AdminMenu() { // Menu Admin 
	char ch;
	clear();
	cout << "\n\n\n\tADMIN MENU: ";
	cout << "\n\n\t01. Add Admins";
	cout << "\n\n\t02. Show Admins";
	cout << "\n\n\t03. Delete Admins";
	cout << "\n\n\t04. Change Password";
	cout << "\n\n\t05. Logout";
	cout << "\n\n\t06. Back To Menu\t";
	cout << "\n\n\tSelect Option: ";
	ch = getche();
	switch(ch){
		case '1':{
			users.AddAdmin();	
			break;
		}
		case '2':{
			users.AdminList();
			break;
		}
		case '3':{
			users.deleteAdmin();
			break;
		}
		case '4':{
			users.changepassword();
			break;
		}
		case '5':{
			users.logout();
			break;
		}
		case '6':{
			break;
		}
		default:{
			cout << "\n\n\tInvalid!";
			break;
		}
	}	
}
void Admin() {
    char ch;
    do {
        clear();
        cout << B << "\n\n\n\t[+] SETTING & PROVACY:" << RE;
        cout << "\n\n\t01. LOGIN";
        cout << "\n\n\t02. FORGET PASSWORD";
        cout << "\n\n\t03. EXIT";
        cout << "\n\n\tSELECT OPTION (1-3): ";
        ch = getche();
        switch (ch) {
            case '1':{
                users.login();
                break;
            }
            case '2':{   
                users.forgetPassword();
                break;
            }
            case '3':{
                cout << "\n\n\tProgram is Exiting...";
                exit(0);
                break;
            }
            default:{
                cout << R << "\n\n\tInvalid Option! Please try again..." << RE;
                getch(); 
                break;
            }
        }
    } while (ch != '1' && ch != '2' && ch != '3' && ch != '0');
}
void Uni() {
	char ch;
	do {
	clear();
	cout << B << "\n\n\n\t[+] UNIVERSITY SECTION:" << RE;
	cout << "\n\n\t01. Add Teacher";
	cout << "\n\n\t02. Update Teacher";
	cout << "\n\n\t03. Delete Teacher";
	cout << "\n\n\t04. Views Teacher";
	cout << "\n\n\t05. Back to menu";
	ch = getche();

    switch (ch) {
        case '1':
            uni.addTeacher();
            break;
        case '2':
            uni.displayTeacher();
            break;
        case '3':
            uni.DeleteTeacher();
            break;
        case '4':
            uni.updateTeacher();
            break;
        case '5':
            cout << "\n\n\tProgram is Exiting...";
        break;
        default:
            cout << R << "\n\n\tInvalid Option! Please try again..." << RE;
            getch(); 
        break;
        }
    }while(ch != '1' && ch != '2' && ch != '3' && ch != '4' && ch != '5');
}
void student(){
	char ch;
	do {
	clear();
	cout << B << "\n\n\n\t[+] STUDENTS SECTION:" << RE;
	cout << "\n\n\t01. Add Students";
	cout << "\n\n\t02. View Students";
	cout << "\n\n\t03. Update Students";
	cout << "\n\n\t04. Delete Students";
	cout << "\n\n\t05. Search Student Info";
	cout << "\n\n\t06. Back to menu";
	ch = getche();
    switch (ch) {
        case '1':
            stu.addStudent();
            break;
        case '2':
            stu.displayStudent();
            break;
        case '3':
            stu.updateStudent();
            break;
        case '4':
            stu.deleteStudent();
            break;
        case '5':
            stu.searchStudents();
            break;
        case '6':
            cout << "\n\n\tProgram is Exiting...";
            break;
        default:
            cout << R << "\n\n\tInvalid Option! Please try again..." << RE;
            getch(); 
        break;
        }
    }while(ch != '1' && ch != '2' && ch != '3' && ch != '4' && ch != '5');    
}
void Enroll(){
	char ch;
	do {
	clear();
	cout << B << "\n\n\n\t[+] ENROLLMENT SECTION:" << RE;
	cout << "\n\n\t01. Enrollments";
	cout << "\n\n\t02. Drop Emrollments";
	cout << "\n\n\t03. View Emrollments";
	cout << "\n\n\t04. Back to menu";
	ch = getche();
    switch (ch) {
        case '1':
            e.enrollStudent();
            break;
        case '2':
            e.dropStudent();
            break;
        case '3':
            e.displayAll();
            break;
        case '4':
            break;
        default:
            cout << R << "\n\n\tInvalid Option! Please try again..." << RE;
            getch(); 
        break;
        }
    }while(ch != '1' && ch != '2' && ch != '3' && ch != '4' && ch != '5');       
}

int main() {
	char ch;
	Admin();
	intro();
	while(true){
		clear();
	 	cout << B << "\n\n\n\tDETAIL MENU:" <<RE;
	 	cout << "\n\n\t01. MANAGE UNIVERSITY";
	 	cout << "\n\n\t02. MANAGE STUDENTS";
	 	cout << "\n\n\t03. MANAGE ENROLLMENT";
	 	cout << "\n\n\t04. CALCULATE GPA";
	 	cout << "\n\n\t05. ADMIN SETTING";
	 	cout << "\n\n\t06. REPORT";
	 	cout << "\n\n\t07. EXIT";
	 	cout << B <<"\n\n\tSELECT OPTION (1-7): " << RE;
	 	ch = getche();
		switch(ch){
			case '1':{
				Uni();
				break;
				}
			case '2':{
				student();	
				break;
				}
			case '3':{
				Enroll();
				break;
			}
			case '4':{
					
				break;
			}
			case '5':{
				AdminMenu();
				break;
			}
			case '6':{
					
				break;
			}
			case '7':{
				cout << G << "\n\n\tSystem is Exiting..." << RE;
				exit(0);
				break;
			}
			default :{
				cout << "\n\n\tInvalid!";
				getch();
				break;
			}
		}
	}
	return 0;
}
