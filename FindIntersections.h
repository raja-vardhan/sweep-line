#include <algorithm>
#include <queue>
#include "StatusQueue.h"
#include "EventQueue.h"
#include <iostream>
#define pdd pair<double, double>

using namespace std;

/// Structure to represent a point
struct Point
{
    double x;
    double y;
};

class FindIntersections
{
    private:
        EventQueue eventQueue;
        EventQueueNode *eventQueueRoot = NULL;
        StatusQueue status = StatusQueue();
        StatusQueueNode *statusRoot = NULL;
    public:

        /// Constructor to initialise event queue and status queue
        FindIntersections( vector<LineSegment> &segmentVector ){
            for(size_t i = 0; i < segmentVector.size(); i++)
            {   
                float startx, starty, endx, endy;
                if(segmentVector[i].startY >= segmentVector[i].endY){
                    startx = segmentVector[i].startX;
                    starty = segmentVector[i].startY;
                    endx = segmentVector[i].endX;
                    endy = segmentVector[i].endY;
                } else {
                    startx = segmentVector[i].endX;
                    starty = segmentVector[i].endY;
                    endx = segmentVector[i].startX;
                    endy = segmentVector[i].startY;
                }
                if (segmentVector[i].startY == segmentVector[i].endY ) {
                    if (segmentVector[i].startX <= segmentVector[i].endX) {
                        startx = segmentVector[i].startX;
                        starty = segmentVector[i].startY;
                        endx = segmentVector[i].endX;
                        endy = segmentVector[i].endY;
                    } else {
                        startx = segmentVector[i].endX;
                        starty = segmentVector[i].endY;
                        endx = segmentVector[i].startX;
                        endy = segmentVector[i].startY;
                    }
                    
                }
                
                // printf("%f %f %f %f\n", startx, starty, endx, endy);             
                
                // insert end points into the EventQueue queue.
                eventQueueRoot = eventQueue.insert( eventQueueRoot, startx, starty, startx, starty, endx, endy, 1);
                eventQueueRoot = eventQueue.insert( eventQueueRoot, endx, endy, startx, starty, endx, endy, 2);
            }
        }


        /// Given three collinear points p, q, r, the function checks if
        /// point q lies on line segment 'pr'.
        bool onSegment(Point p, Point q, Point r) 
        { 
            if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && 
                q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) 
            return true; 
        
            return false; 
        } 


        /// To find orientation of ordered triplet (p, q, r). 
        ///
        /// @returns 0 if p, q and r are collinear 
        /// @returns 1 if they are in clockwise orientation
        /// @returns 2 if they are in ounterclockwise orientation
        int orientation(Point p, Point q, Point r) 
        { 
            float val = (q.y - p.y) * (r.x - q.x) - 
                    (q.x - p.x) * (r.y - q.y); 
        
            if (val == 0) return 0;  // collinear 
        
            return (val > 0)? 1: 2; // clock or counterclock wise 
        } 


       /// Check if two line segments 'l1' and 'l2' intersect.
       /// @returns *true* if they intersect
       /// @returns *false* if they do not intersect
        bool doIntersect(LineSegment l1, LineSegment l2)
        { 
            struct Point p1, q1, p2, q2;
            p1.x = l1.startX;
            p1.y = l1.startY;
            q1.x = l1.endX;
            q1.y = l1.endY;
            p2.x = l2.startX;
            p2.y = l2.startY;
            q2.x = l2.endX;
            q2.y = l2.endY;
            // Find the four orientations needed for general and special cases 
            int o1 = orientation(p1, q1, p2); 
            int o2 = orientation(p1, q1, q2); 
            int o3 = orientation(p2, q2, p1); 
            int o4 = orientation(p2, q2, q1); 
        
            // General case 
            if (o1 != o2 && o3 != o4) 
                return true; 
        
            // Special Cases 
            // p1, q1 and p2 are collinear and p2 lies on segment p1q1 
            if (o1 == 0 && onSegment(p1, p2, q1)) return true; 
        
            // p1, q1 and q2 are collinear and q2 lies on segment p1q1 
            if (o2 == 0 && onSegment(p1, q2, q1)) return true; 
        
            // p2, q2 and p1 are collinear and p1 lies on segment p2q2 
            if (o3 == 0 && onSegment(p2, p1, q2)) return true; 
        
            // p2, q2 and q1 are collinear and q1 lies on segment p2q2 
            if (o4 == 0 && onSegment(p2, q1, q2)) return true; 
        
            return false; // Doesn't fall in any of the above cases 
        } 

        /// Find the intersection point of two line segments if they intersect
        Point intersectionOf(LineSegment l1, LineSegment l2){
            
            Point intersection;
            if (doIntersect(l1, l2) == 0) 
            { 
                // The line segments do not intersect.
                intersection.x = -1;
                intersection.y = -1;
            } 
            else
            {   
                // Line l1 represented as a1x + b1y = c1 
                double a1 = l1.endY - l1.startY; 
                double b1 = l1.startX - l1.endX; 
                double c1 = a1*(l1.startX) + b1*(l1.startY); 
            
                // Line l2 represented as a2x + b2y = c2 
                double a2 = l2.endY - l2.startY; 
                double b2 = l2.startX - l2.endX; 
                double c2 = a2*(l2.startX) + b2*(l2.startY); 
            
                double determinant = a1*b2 - a2*b1; 
                intersection.x = (b2*c1 - b1*c2)/determinant; 
                intersection.y = (a1*c2 - a2*c1)/determinant;
            }
            return intersection;
        }

        /// Insert the new event point resulting from the intersection of two line segments 'sl' and 'sr'
        void findNewEvent(LineSegment sl, LineSegment sr, EventQueueNode* p){
            // find intersection Point of sl and sr
            struct Point newEventPoint = intersectionOf(sl, sr);
            // printf("intersection Point of %f %f %f %f AND %f %f %f %f: %f %f\n", sl.startX, sl.startY, sl.endX, sl.endY, sr.startX, sr.startY, sr.endX, sr.endY, newEventPoint.x, newEventPoint.y);
            if (newEventPoint.y != -1) {
                if(newEventPoint.y < p->yc){
                    eventQueueRoot = eventQueue.insert( eventQueueRoot, newEventPoint.x, newEventPoint.y, sl.startX, sl.startY, sl.endX, sl.endY, 3);
                    eventQueueRoot = eventQueue.insert( eventQueueRoot, newEventPoint.x, newEventPoint.y, sr.startX, sr.startY, sr.endX, sr.endY, 3);
                } else if(newEventPoint.y == p->yc && newEventPoint.x > p->xc){
                    eventQueueRoot = eventQueue.insert( eventQueueRoot, newEventPoint.x, newEventPoint.y, sl.startX, sl.startY, sl.endX, sl.endY, 3);
                    eventQueueRoot = eventQueue.insert( eventQueueRoot, newEventPoint.x, newEventPoint.y, sr.startX, sr.startY, sr.endX, sr.endY, 3);
                }
            }
            
        }

        /// Check if a line segment is in a vector of line segments
        /// @param x Vector of line segments
        /// @param l Line segment to be checked
        /// @returns 0 if the vector x contains line segment l
        /// @returns 1 if the vector x doesn't contain line segment l
        int contains(vector<LineSegment> x, LineSegment l){
            for(size_t i = 0; i < x.size(); i++)
            {
                if(x[i].startX == l.startX && x[i].startY == l.startY && x[i].endX == l.endX && x[i].endY == l.endY){
                    return 0;
                }
            }
            return 1;
        }

        /// Find the union of two vectors of line segments 'a' and 'b'
        /// @returns Vector containing all line segments in the union of 'a' and 'b'
        vector<LineSegment> unionOf(vector<LineSegment> a, vector<LineSegment> b){
            vector<LineSegment> unionVec;
            for(size_t i = 0; i < a.size(); i++)
            {
                unionVec.push_back(a[i]);
            }
            for(size_t i = 0; i < b.size(); i++)
            {
                if(contains(unionVec, b[i]) == 1){
                    unionVec.push_back(b[i]);
                }
            }
            return unionVec;
        }

        // Check if a vector of line segments 'x' is empty
        // int empty(vector<LineSegment> x){
        //     if(x.size() == 0){
        //         return 1;
        //     } else {
        //         return 0;
        //     }
        // }

        /// Handle each event queue point popped from the event queue
        void handleEventPoint(EventQueueNode* eventPoint){

            // Union of Lp, Up and Cp
            vector<LineSegment> all = unionOf(eventPoint->L, unionOf(eventPoint->U, eventPoint->C));

            if (all.size() > 1) {
                // p is an intersection
                printf("Intersection: %f %f\n", eventPoint->xc, eventPoint->yc);
            }
            // delete elements of Lp union Cp from status
            vector<LineSegment> temp1 = unionOf(eventPoint->L, eventPoint->C);
            for(size_t i = 0; i < temp1.size(); i++)
            {
                // printf("delete line: %f %f %f %f\n",temp1[i].startX,temp1[i].startY, temp1[i].endX, temp1[i].endY);
                statusRoot = status.deleteNode(statusRoot, temp1[i], eventPoint->yc);
            }
            // printf("Point: %f %f\n", eventPoint->xc, eventPoint->yc);
            // printf("after deleting:\n");
            // status.preOrder(statusRoot);
            
            // insert segments in Up union Cp into status according to their position just below the sweep line
            vector<LineSegment> temp2 = unionOf(eventPoint->U, eventPoint->C);
            for(size_t i = 0; i < temp2.size(); i++)
            {
                statusRoot = status.insert(statusRoot, temp2[i], (eventPoint->yc - 0.1));
            }
            // printf("after reinserting:\n");
            // status.preOrder(statusRoot);

            // check if Up union Cp is empty
            if(temp2.empty() == 1){
                struct LineSegment sl, sr;
                sl.startX = -1;
                sr.startX = -1;
                // status.preOrder(statusRoot);
                status.getNeighbors(statusRoot, eventPoint->xc, (eventPoint->yc)-0.1, &sl, &sr);
                // if(sl.startX != -1){
                //     if (sr.startX != -1) {
                //         findNewEvent(sl, sr, eventPoint);
                //     }
                // }
            } else {
                struct LineSegment sll, srr;
                float max = -1.0, min = 1001.0; 
                for(size_t i = 0; i < temp2.size(); i++)
                {
                    float x = status.findx(temp2[i], eventPoint->yc);
                    float minX = (temp2[i].startX < temp2[i].endX) ? temp2[i].startX : temp2[i].endX;
                    float maxX = (temp2[i].startX > temp2[i].endX) ? temp2[i].startX : temp2[i].endX;
                    if (x < min && x <= maxX && x >= minX) {
                        min = x;
                        sll = temp2[i];
                    }
                    if(x > max  && x <= maxX && x >= minX){
                        
                        max = x;
                        srr = temp2[i];
                    }
                    
                }
  
                struct LineSegment sl, sr;
                sl.startX = -1;
                sr.startX = -1;
                status.getLeftNeighbor(statusRoot, sll, eventPoint->yc, &sl);
                status.getRightNeighbor(statusRoot , srr, eventPoint->yc, &sr);
                
                if(sl.startX != -1 && min != 1001){
                    findNewEvent(sl, sll, eventPoint);
                }
                if(sr.startX != -1 && max != -1){
                    findNewEvent(srr, sr, eventPoint);
                }
            }
            

        }

        /// Run the algorithm to find the line intersections
        void runAlgorithm(){
            while(eventQueueRoot != NULL){
                EventQueueNode* pop = eventQueue.maxValueNode(eventQueueRoot);
                if (pop != NULL) {
                   handleEventPoint(pop); 
                   eventQueueRoot = eventQueue.deleteNode(eventQueueRoot, pop->xc, pop-> yc);
                }
                
            }
            cout << "\nExecution complete\n";
        }











































































































































































































































































































    pdd lineLineIntersection(pdd A, pdd B, pdd C, pdd D)
  {
    // Line AB represented as a1x + b1y = c1
    double a1 = B.second - A.second;
    double b1 = A.first - B.first;
    double c1 = a1 * (A.first) + b1 * (A.second);

    // Line CD represented as a2x + b2y = c2
    double a2 = D.second - C.second;
    double b2 = C.first - D.first;
    double c2 = a2 * (C.first) + b2 * (C.second);

    double determinant = a1 * b2 - a2 * b1;

    if (determinant == 0)
    {
      // The lines are parallel. This is simplified
      // by returning a pair of FLT_MAX
      return make_pair(-1, -1);
    }
    else
    {
      double x = (b2 * c1 - b1 * c2) / determinant;
      double y = (a1 * c2 - a2 * c1) / determinant;
      return make_pair(x, y);
    }
  }

  void runAlgorithmB(vector<LineSegment> &segmentVector)
  {
    int n = (int)segmentVector.size();

    for (int i = 0; i < n; i++)
    {
      for (int j = i + 1; j < n; j++)
      {
        pdd A, B, C, D;
        A.first = segmentVector[i].startX;
        A.second = segmentVector[i].startY;
        B.first = segmentVector[i].endX;
        B.second = segmentVector[i].endY;
        C.first = segmentVector[j].startX;
        C.second = segmentVector[j].startY;
        D.first = segmentVector[j].endX;
        D.second = segmentVector[j].endY;

        pdd intersection = lineLineIntersection(A, B, C, D);

        if (intersection.first == -1 &&
            intersection.second == -1)
        {
          continue;
        }

        else
        {
          // && (min(A.second, B.second) <= intersection.first <= max(A.second, B.second))
          if (min(A.first, B.first) <= intersection.first
           && intersection.first <= max(A.first, B.first)) {
            if (min(A.second, B.second) <= intersection.second &&
                intersection.second <= max(A.second, B.second)){

            }
            else{
              continue;
            }
          

          }
          else{
          continue;
          }

          // NOTE: Further check can be applied in case
          // of line segments. Here, we have considered AB
          // and CD as lines
          
          cout << "The intersection point is : (" << intersection.first << "," << intersection.second<< ")"<< endl;

          
        }
      }
    }
  }

};