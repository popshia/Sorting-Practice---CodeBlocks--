// 10627130 資工二甲 林冠良 & 10627131 資工二甲 李峻瑋 // CodeBlocks 17.12
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <time.h>

using namespace std ;

typedef struct DataStruct {
    string schoolNum = "\0" ;
    string schoolName = "\0" ;
    string departmentNum = "\0" ;
    string departmentName = "\0" ;
    int student = 0 ;
    int teacher = 0 ;
    int graduated = 0 ;
    string wholeSentence = "\0" ;
} DataStruct ;

static ifstream input ;
static ofstream output ;
static string FileN = "0" ;
static int Count = 0 ;

class DestroyDickDecember {
    vector<DataStruct> dataBase ;
    vector<DataStruct> sorted ;

public:
    void inputData() { // tool
        dataBase.clear() ;
        sorted.clear() ;
        DataStruct tempData ;
        string sentence = "\0" ;
        getline( input, sentence ) ;
        getline( input, sentence ) ;
        getline( input, sentence ) ;

        while ( getline( input, sentence ) ) {
            // cout << sentence << endl ;
            tempData.wholeSentence = sentence ;
            vector<string> cut ;
            string token ;
            istringstream cutStream( sentence ) ;

            while ( getline( cutStream, token, '\t' ) ) // cut the token one by one
                cut.push_back( token ) ;

            tempData.schoolNum = cut[0] ;
            tempData.schoolName = cut[1] ;
            tempData.departmentNum = cut[2] ;
            tempData.departmentName = cut[3] ;

            if ( cut[6].size() > 3 ) {
                cut[6].erase( find( cut[6].begin(), cut[6].end(), '"' ) ) ;
                cut[6].erase( find( cut[6].begin(), cut[6].end(), ',' ) ) ;
                cut[6].erase( find( cut[6].begin(), cut[6].end(), '"' ) ) ;
            } // erase '"' & ','
            tempData.student = atoi( cut[6].c_str() ) ;

            if ( cut[8].size() > 3 ) {
                cut[8].erase( find( cut[8].begin(), cut[8].end(), '"' ) ) ;
                cut[8].erase( find( cut[8].begin(), cut[8].end(), ',' ) ) ;
                cut[8].erase( find( cut[8].begin(), cut[8].end(), '"' ) ) ;
            } // erase '"' & ','
            tempData.graduated = atoi( cut[8].c_str() ) ;
            dataBase.push_back( tempData ) ;
            Count++ ;
        } // get the whole file
    } // input the data

    int FindLargest() { // tool
        int largestIndex = 0 ;
        for ( int i = 0 ; i < dataBase.size() ; i++ ) {
            if ( dataBase[i].graduated > dataBase[largestIndex].graduated ) largestIndex = i ;
        }
        return largestIndex ;
    } // find the largest

    int FindSmallest() { // tool
        int smallestIndex = 0 ;
        for ( int i = 0 ; i < dataBase.size() ; i++ ) {
            if ( dataBase[i].graduated < dataBase[smallestIndex].graduated ) smallestIndex = i ;
        }
        return smallestIndex ;
    } // find the smallest

    void SelectionSort() { // done
        while ( ! dataBase.empty() ) {
            int largestIndex = FindLargest() ;
            // cout << dataBase[largestIndex].graduated << endl ;
            sorted.push_back( dataBase[largestIndex] ) ;
            dataBase.erase( dataBase.begin()+largestIndex ) ;
        } // add to new vector and erase
    } // function 1: Selection Sort (done)

    void BubbleSort() { // done
        for ( int one = 0 ; one < dataBase.size() ; one++ ) {
            // cout << "one: " << dataBase[one].graduated << endl ;
            for ( int two = one ; two < dataBase.size() ; two ++ ) {
                // cout << "two: " << dataBase[two].graduated << endl ;
                if ( dataBase[two].graduated > dataBase[one].graduated ) swap( dataBase[two] ,dataBase[one] ) ;
            } // for
        } // for
        for ( int i = 0 ; i < dataBase.size() ; i++ ) output << dataBase[i].wholeSentence << endl ;
    } // function 1: Bubble Sort (done)

    void Merge( int front, int mid, int end ) { // done
        vector<DataStruct> left( dataBase.begin()+front, dataBase.begin()+mid+1 ) ;
        vector<DataStruct> right( dataBase.begin()+mid+1, dataBase.begin()+end+1 ) ;
        DataStruct max ;
        max.graduated = 99999 ;

        left.push_back( max ) ;
        right.push_back( max ) ;

        int leftIndex = 0 ;
        int rightIndex = 0 ;

        for ( int i = front ; i <= end ; i++ ) {
            // cout << left[leftIndex].graduated << " " << right[rightIndex].graduated << endl ;
            if ( left[leftIndex].graduated <= right[rightIndex].graduated ) {
                dataBase[i] = left[leftIndex] ;
                leftIndex++ ;
            } // left
            else {
                dataBase[i] = right[rightIndex] ;
                rightIndex++ ;
            } // right
        } // compare two sub vectors
    } // merge left and right sub vectors

    void MergeSort( int front, int end ) { // done
        if ( front < end ) {
            int mid = ( front + end ) / 2 ;
            MergeSort( front, mid ) ;
            MergeSort( mid+1, end ) ;
            Merge( front, mid, end ) ;
        } // run recursive

        sorted = dataBase ;
        reverse( sorted.begin(), sorted.end() ) ;
    } // function 2: Merge Sort

    int Partition( int front, int end ) { // done
        int pivot = dataBase[end].graduated ;
        int i = front - 1 ;

        for ( int j = front ; j < end ; j++ ) {
            if ( dataBase[j].graduated < pivot ) {
                i++ ;
                swap( dataBase[i], dataBase[j] ) ;
            } // if smaller, swap
        } // run the whole data

        i++ ;
        swap( dataBase[i], dataBase[end] ) ;
        return i ;
    } // Partition

    void QuickSort( int front, int end ) { // done
        if ( front < end ) {
            int pivot = Partition( front, end ) ;
            QuickSort( front, pivot-1 ) ;
            QuickSort( pivot+1, end ) ;
        } // run recursive

        /*cout << "Sorted: " ;
        for ( int i = 0 ; i < dataBase.size() ; i++ ) cout << dataBase[i].graduated << " " ;
        cout << endl ;*/
        sorted = dataBase ;
        reverse( sorted.begin(), sorted.end() ) ;
    } // function 2: Quick Sort

    void RadixSort() { // done
        DataStruct temp[Count][Count] = {} ;
        int order[Count] = {0} ;
        int n = 1 ;
        int largest = dataBase[FindLargest()].graduated ;
        int digit = 1 ;

        while ( n <= dataBase[FindLargest()].graduated ) {
            int i ;
            for ( i = 0 ; i < Count ; i++ ) {
                int lsd = ( ( dataBase[i].graduated / n ) % 10 ) ;
                temp[lsd][order[lsd]] = dataBase[i] ;
                order[lsd]++ ;
            } // first category

            int k = 0 ;
            for ( i = 0 ; i < Count ; i++ ) {
                if ( order[i] != 0 ) {
                    int j ;
                    for ( j = 0 ; j < order[i] ; j++, k++ ) dataBase[k] = temp[i][j] ;
                } // get the one out
                    order[i] = 0 ;
            } // reorder

            n *= 10 ;
        } // run the whole data

        sorted = dataBase ;
        reverse( sorted.begin(), sorted.end() ) ;
        /*cout << "Sorted: " ;
        for ( int i = 0 ; i < dataBase.size() ; i++ ) cout << dataBase[i].graduated << " " ;
        cout << endl ;*/
    } // function 3: Radix Sort

    void Print() { for( int i = 0 ; i < sorted.size() ; i++ ) output << sorted[i].wholeSentence << endl ; } // Print out sorted data
} ; // DestroyDickDecember

int main() {
    int command = 0 ;
    bool continueOrNot = false ;
    DestroyDickDecember function1 ;
    DestroyDickDecember function2 ;
    DestroyDickDecember function3 ;

    do {
        cout << "*****************************************" << endl ; // welcome message
        cout << "*****        Sorting Practice       *****" << endl ;
        cout << "***** 0 : Quit                      *****" << endl ;
        cout << "***** 1 : Select & Bubble Sort      *****" << endl ;
        cout << "***** 2 : Merge & Quick Sort        *****" << endl ;
        cout << "***** 3 : Radix Sort                *****" << endl ;
        cout << "*****************************************" << endl ;
        cout << endl << "Please enter your choice :" << endl ;

        cin >> command ; // read in user command

        if ( command == 0 ) { // bye :(((
            cout << "Bye :(((" << endl ;
            return 0 ;
        } // quit

        else if ( command > 3 || command < 0 ) {
            cout << "Error command! please enter an acceptable command :" << endl << endl ;
            continueOrNot = true ;
        } // wrong command

        else if ( command == 1 ) {
            bool function1Confirm = false ;

            do {
                cout << "Please enter the file you want to read and copy or [0] to quit:" << endl ;
                cin >> FileN ;

                if ( FileN == "0" ) {
                    function1Confirm = true ;
                    continueOrNot = true ;
                } // quit

                else {
                    string fileName = "input" + FileN + ".txt" ;
                    input.open( fileName.c_str() ) ;
                    // cut the input FileN, try to open
                    if ( input.is_open() ) {
                        function1.inputData() ;
                        clock_t time ;
                        // selection sort
                        time = clock() ;
                        function1.SelectionSort() ; // sort
                        string outputName = "select_sort" + FileN + ".txt" ;
                        output.open( outputName.c_str() ) ;
                        function1.Print() ; // print
                        input.close() ;
                        output.close() ;
                        time = clock() - time ;
                        cout << "Selection Sort cost time = " << time * 1000 / CLOCKS_PER_SEC << " ms" << endl ; // print out the time
                        // bubble sort
                        input.open( fileName.c_str() ) ;
                        function1.inputData() ;
                        time = clock() ;
                        outputName = "bubble_sort" + FileN + ".txt" ;
                        output.open( outputName.c_str() ) ;
                        function1.BubbleSort() ;
                        input.close() ;
                        output.close() ;
                        time = clock() - time ;
                        cout << "Bubble Sort cost time = " << time * 1000 / CLOCKS_PER_SEC << " ms" << endl ; // print out the time
                        // set parameters
                        function1Confirm = true ;
                        continueOrNot = true ;
                    } // successfully opened
                    else cout << "*****  " << fileName << " does not exist!  *****" << endl ; // no file
                } // open and sort
            } while( ! function1Confirm ) ;

            FileN = "0" ;
        } // mission 1: Select & Bubble Sort

        else if ( command == 2 ) {
            bool function2Confirm = false ;

            do {
                cout << "Please enter the file you want to read and copy or [0] to quit:" << endl ;
                cin >> FileN ;

                if ( FileN == "0" ) {
                    function2Confirm = true ;
                    continueOrNot = true ;
                } // quit

                else {
                    string fileName = "input" + FileN + ".txt" ;
                    input.open( fileName.c_str() ) ;
                    // cut the input FileN, try to open
                    if ( input.is_open() ) {
                        function2.inputData() ;
                        clock_t time ;
                        // merge sort
                        time = clock() ;
                        string outputName = "merge_sort" + FileN + ".txt" ;
                        output.open( outputName.c_str() ) ;
                        function2.MergeSort( 0, Count-1 ) ; // sort
                        function2.Print() ;
                        input.close() ;
                        output.close() ;
                        time = clock() - time ;
                        cout << "Merge Sort cost time = " << time * 1000 / CLOCKS_PER_SEC << " ms" << endl ; // print out the time
                        // quick sort
                        Count = 0 ;
                        input.open( fileName.c_str() ) ;
                        function2.inputData() ;
                        time = clock() ;
                        outputName = "quick_sort" + FileN + ".txt" ;
                        output.open( outputName.c_str() ) ;
                        function2.QuickSort( 0, Count-1 ) ; // sort
                        function2.Print() ;
                        input.close() ;
                        output.close() ;
                        time = clock() - time ;
                        cout << "Quick Sort cost time = " << time * 1000 / CLOCKS_PER_SEC << " ms" << endl ; // print out the time
                        // set parameters
                        function2Confirm = true ;
                        continueOrNot = true ;
                    } // successfully opened
                    else cout << "*****  " << fileName << " does not exist!  *****" << endl ; // no file
                } // open and sort
            } while( ! function2Confirm ) ;

            FileN = "0" ;
        } // mission 2: Merge & Quick Sort

        else if ( command == 3 ) {
            bool function3Confirm = false ;

            do {
                cout << "Please enter the file you want to read and copy or [0] to quit:" << endl ;
                cin >> FileN ;

                if ( FileN == "0" ) {
                    function3Confirm = true ;
                    continueOrNot = true ;
                } // quit

                else {
                    string fileName = "input" + FileN + ".txt" ;
                    input.open( fileName.c_str() ) ;
                    if ( input.is_open() ) {
                        function3.inputData() ;
                        clock_t time ;
                        time = clock() ;
                        string outputName = "radix_sort" + FileN + ".txt" ;
                        output.open( outputName.c_str() ) ;
                        function3.RadixSort() ;
                        function3.Print() ;
                        time = clock() - time ;
                        cout << "Radix Sort cost time = " << time * 1000 / CLOCKS_PER_SEC << " ms" << endl ; // print out the time
                        function3Confirm = true ;
                        continueOrNot = true ;
                    } // open successfully
                    else cout << "*****  input" << FileN << ".txt does not exist!  *****" << endl ; // no file
                } // open and sort
            } while( ! function3Confirm ) ;

            Count = 0 ;
            FileN = "0" ;
            input.close() ;
            output.close() ;
        } // mission 3: Radix Sort
    } while( continueOrNot ) ;
} // main function
