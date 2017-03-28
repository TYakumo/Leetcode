struct IntervalInfo2D
{
    int startX, endX;
    int startY, endY;
    IntervalInfo2D() {}
    IntervalInfo2D(int sx, int ex, int sy, int ey) : startX(sx), endX(ex), startY(sy), endY(ey) {}

    bool operator==(const IntervalInfo2D& comp) const
    {
        return startX == comp.startX && endX == comp.endX && startY == comp.startY && endY == comp.endY;
    }
};

class SegmentTree2DNode
{
    public:
    int currentSum;
    IntervalInfo2D intervalInfo;

    SegmentTree2DNode *leftUpperChild;
    SegmentTree2DNode *leftDownChild;
    SegmentTree2DNode *rightUpperChild;
    SegmentTree2DNode *rightDownChild;

    SegmentTree2DNode()
    {
        leftUpperChild = nullptr;
        leftDownChild = nullptr;
        rightUpperChild = nullptr;
        rightDownChild = nullptr;
        currentSum = 0;
    }
};

class SegmentTree2D
{
    private:      
    SegmentTree2DNode *mRootNode;
    void updateInterval_(SegmentTree2DNode *, IntervalInfo2D , int);
    int getIntervalSum_(SegmentTree2DNode *, IntervalInfo2D);
    void initTree_(SegmentTree2DNode *);

    public:
    SegmentTree2D()
    {
        mRootNode = new SegmentTree2DNode();
    }

    void initTree(IntervalInfo2D);
    void updateInterval(IntervalInfo2D, int);
    int getIntervalSum(IntervalInfo2D);
};

void SegmentTree2D::initTree_(SegmentTree2DNode *currentNode)
{
    IntervalInfo2D &nodeInterval = currentNode->intervalInfo;      

    if (nodeInterval.startX < nodeInterval.endX && nodeInterval.startY < nodeInterval.endY)
    {               
        int midX = (nodeInterval.startX+nodeInterval.endX)/2;
        int midY = (nodeInterval.startY+nodeInterval.endY)/2;        

        currentNode->leftUpperChild = new SegmentTree2DNode();
        currentNode->leftUpperChild->intervalInfo.startX = nodeInterval.startX;
        currentNode->leftUpperChild->intervalInfo.endX = midX;
        currentNode->leftUpperChild->intervalInfo.startY = nodeInterval.startY;
        currentNode->leftUpperChild->intervalInfo.endY = midY;
        initTree_(currentNode->leftUpperChild);

        currentNode->rightUpperChild = new SegmentTree2DNode();
        currentNode->rightUpperChild->intervalInfo.startX = midX+1;
        currentNode->rightUpperChild->intervalInfo.endX = nodeInterval.endX;
        currentNode->rightUpperChild->intervalInfo.startY = nodeInterval.startY;
        currentNode->rightUpperChild->intervalInfo.endY = midY;    
        initTree_(currentNode->rightUpperChild);

        currentNode->leftDownChild = new SegmentTree2DNode();
        currentNode->leftDownChild->intervalInfo.startX = nodeInterval.startX;
        currentNode->leftDownChild->intervalInfo.endX = midX;
        currentNode->leftDownChild->intervalInfo.startY = midY+1;
        currentNode->leftDownChild->intervalInfo.endY = nodeInterval.endY;
        initTree_(currentNode->leftDownChild);

        currentNode->rightDownChild = new SegmentTree2DNode();
        currentNode->rightDownChild->intervalInfo.startX = midX+1;
        currentNode->rightDownChild->intervalInfo.endX = nodeInterval.endX;
        currentNode->rightDownChild->intervalInfo.startY = midY+1;
        currentNode->rightDownChild->intervalInfo.endY = nodeInterval.endY;
        initTree_(currentNode->rightDownChild);
    }
}

void SegmentTree2D::initTree(IntervalInfo2D intervalInfo)
{
    mRootNode->intervalInfo = intervalInfo;
    initTree_(mRootNode);
}

void SegmentTree2D::updateInterval_(SegmentTree2DNode *currentNode, IntervalInfo2D intervalInfo, int updateValueDifference)
{
    currentNode->currentSum += (updateValueDifference);
    if (intervalInfo == currentNode->intervalInfo)
    {
        return ;
    }

    int midX = (currentNode->intervalInfo.startX+currentNode->intervalInfo.endX)/2;
    int midY = (currentNode->intervalInfo.startY+currentNode->intervalInfo.endY)/2;
        
    if (intervalInfo.endX <= midX && intervalInfo.endY <= midY) {       
        updateInterval_(currentNode->leftUpperChild, intervalInfo, updateValueDifference);
    }
    else if (intervalInfo.startX > midX && intervalInfo.endY <= midY) {
        updateInterval_(currentNode->rightUpperChild, intervalInfo, updateValueDifference);
    }
    else if (intervalInfo.startX <= midX && intervalInfo.startY > midY) {
        updateInterval_(currentNode->leftDownChild, intervalInfo, updateValueDifference);
    }
    else {
        updateInterval_(currentNode->rightDownChild, intervalInfo, updateValueDifference);
    }
}

int SegmentTree2D::getIntervalSum_(SegmentTree2DNode *currentNode, IntervalInfo2D intervalInfo)
{    
    if (intervalInfo == currentNode->intervalInfo)
    {
        return currentNode->currentSum;
    }

    int midX = (currentNode->intervalInfo.startX+currentNode->intervalInfo.endX)/2;
    int midY = (currentNode->intervalInfo.startY+currentNode->intervalInfo.endY)/2;

    if (intervalInfo.startX <= midX && intervalInfo.endX > midX)
    {
        return getIntervalSum_(currentNode, IntervalInfo2D(intervalInfo.startX, midX, intervalInfo.startY, intervalInfo.endY)) 
               + getIntervalSum_(currentNode, IntervalInfo2D(midX + 1, intervalInfo.endX, intervalInfo.startY, intervalInfo.endY));
    }
    
    if (intervalInfo.startY <= midY && intervalInfo.endY > midY)
    {
        return getIntervalSum_(currentNode, IntervalInfo2D(intervalInfo.startX, intervalInfo.endX, intervalInfo.startY, midY)) 
               + getIntervalSum_(currentNode, IntervalInfo2D(intervalInfo.startX, intervalInfo.endX, midY+1, intervalInfo.endY));
    }

    if (intervalInfo.endX <= midX && intervalInfo.endY <= midY) {
        return getIntervalSum_(currentNode->leftUpperChild, intervalInfo);
    }
    else if (intervalInfo.startX > midX && intervalInfo.endY <= midY) {
        return getIntervalSum_(currentNode->rightUpperChild, intervalInfo);
    }
    else if (intervalInfo.startX <= midX && intervalInfo.startY > midY) {
        return getIntervalSum_(currentNode->leftDownChild, intervalInfo);
    }
    else {
        return getIntervalSum_(currentNode->rightDownChild, intervalInfo);
    }
}

void SegmentTree2D::updateInterval(IntervalInfo2D intervalInfo, int updateValueDifference)
{
    updateInterval_(mRootNode, intervalInfo, updateValueDifference);
}

int SegmentTree2D::getIntervalSum(IntervalInfo2D intervalInfo)
{
    return getIntervalSum_(mRootNode, intervalInfo);
}

int toClosestPowerOfTwo(int val)
{
    int ret = 0;
    
    while ((1<<ret) < val)
        ++ret;
    
    return (1<<ret);
}

class NumMatrix {
public:
    SegmentTree2D mSGT2D;
    vector< vector<int> > mBackupMatrix;

    NumMatrix(vector<vector<int>> matrix) {     
        if(0 != matrix.size())
        {
            int treeBound = max(toClosestPowerOfTwo(matrix.size()), toClosestPowerOfTwo(matrix[0].size()))-1;
            mSGT2D.initTree(IntervalInfo2D(0, treeBound, 0, treeBound));
        }
        
        mBackupMatrix = matrix;
        
        for (int i = 0; i < matrix.size(); ++i)
            for (int j = 0; j < matrix[0].size(); ++j)
                mSGT2D.updateInterval(IntervalInfo2D(i,i,j,j), matrix.at(i).at(j));
    }

    void update(int row, int col, int val) {
        IntervalInfo2D intervalInfo(row, row, col, col);
        int oriValue = mBackupMatrix[row][col];
        mSGT2D.updateInterval(intervalInfo, val-oriValue);
        mBackupMatrix[row][col] = val;
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        return mSGT2D.getIntervalSum(IntervalInfo2D(row1, row2, col1, col2));
    }
};
