#include "FindIntersections.h"

int main(){
    vector<LineSegment> segmentVector;
    cout << "Enter the number of lines you want to add : ";
    int n;
    cin >> n;
    double x1, y1, x2, y2;
    for(int i=0;i<n;i++)
    {
        cin >> x1 >> y1 >> x2 >> y2;
        LineSegment l1;
        l1.startX = x1;
        l1.startY = y1;
        l1.endX = x2;
        l1.endY = y2;
        segmentVector.push_back(l1);
    }

    // cout << endl << endl;
    // for(int i=0;i<n;i++)
    // {
    //     cout << segmentVector[i].startX << " " << segmentVector[i].startY << " " << segmentVector[i].endX << " " << segmentVector[i].endY << endl;
    // }
    
    cout << endl << "Points of intersections are : \n";
    FindIntersections findIntersection = FindIntersections(segmentVector);
    findIntersection.runAlgorithm();
}