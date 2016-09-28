/******************************************************************** 

Purpose  : 封装四叉树的基础实现 

实现四叉树的自动构建，可按需派生实现自定义自动构建
实现四叉树的检索，可按需派生实现自定义检索

Version  : 1.0 
*********************************************************************/
#pragma once

#include <vector>

using namespace std;

namespace QTree
{

    typedef unsigned short ushort;
    typedef unsigned long ulong;

    // Effect: - 矩形坐标范围
    // TCoor : - 坐标的数据类型，如int、float、double等数值型
    // precision 坐标计算的精度
    // Brief : - 
    template<typename TCoor, int precision>
    class CRect
    {
    public:

        CRect()
            : _xmin(0)
            , _ymin(0)
            , _xmax(0)
            , _ymax(0)
        {
            dpresicion = 1.0 / precision;
        }

        explicit CRect(TCoor xmin, TCoor ymin, TCoor xmax, TCoor ymax)
            : _xmin(xmin)
            , _ymin(ymin)
            , _xmax(xmax)
            , _ymax(ymax)
        {
            dpresicion = 1.0 / precision;
        }

        virtual ~CRect()
        {

        }

        // Effect: - 判断两个数据是否相等
        // In    : - 
        // In    : - 
        // Out   : - 
        // Rtn   : - 
        // Brief : - 如果有特殊需求，请按需重写即可
        virtual bool Equal(TCoor a, TCoor b) const
        {
            return fabs(a - b) < dpresicion;
        }

        // Effect: - 获取左上角拆分范围
        // Rtn   : - 
        // Brief : - 
        CRect GetLTRect() const
        {
            return CRect(_xmin, (_ymax + _ymin) / 2, (_xmin +_xmax) / 2, _ymax);
        }

        // Effect: - 获取右上角拆分范围
        // Rtn   : - 
        // Brief : - 
        CRect GetRTRect() const
        {
            return CRect((_xmin +_xmax) / 2, (_ymax + _ymin) / 2, _xmax, _ymax);
        }

        // Effect: - 获取左下角拆分范围
        // Rtn   : - 
        // Brief : - 
        CRect GetLBRect() const
        {
            return CRect(_xmin, _ymin, (_xmin +_xmax) / 2,  (_ymax + _ymin) / 2);
        }

        // Effect: - 获取右下角拆分范围
        // Rtn   : - 
        // Brief : - 
        CRect GetRBRect() const
        {
            return CRect((_xmin +_xmax) / 2, _ymin, _xmax,  (_ymax + _ymin) / 2);
        }

        // Effect: - 判断指定的点位是否在举行范围内
        // In    : - 
        // In    : - 
        // Rtn   : - 
        // Brief : - 
        bool Inter(TCoor x, TCoor y) const
        {
            return 
                BigEqual(x, _xmin) &&
                LessEqual(x, _xmax) && 
                BigEqual(y, _ymin) &&
                LessEqual(y, _ymax);
        }

        // Effect: - >=
        // In    : - 
        // In    : - 
        // Rtn   : - 
        // Brief : - 
        bool BigEqual(TCoor a, TCoor b) const
        {
            if (Equal(a, b))
            {
                return true;
            }

            return a > b;
        }

        // Effect: - <=
        // In    : - 
        // In    : - 
        // Rtn   : - 
        // Brief : - 
        bool LessEqual(TCoor a, TCoor b) const
        {
            if (Equal(a, b))
            {
                return true;
            }

            return a < b;
        }

        // Effect: - 判断指定的矩形范围是否和当前范围相交
        // In    : - 
        // Rtn   : - 
        // Brief : - 
        bool Inter(const CRect& rhs) const
        {
            return 
                LessEqual(_xmax + _xmin - rhs._xmax - rhs._xmin, _xmax - _xmin + rhs._xmax - rhs._xmin) &&
                LessEqual(_ymax + _ymin - rhs._ymax - rhs._ymin, _ymax - _ymin + rhs._ymax - rhs._ymin);
        }

        // Effect: - 取绝对值
        // In    : - 
        // Rtn   : - 
        // Brief : - 
        TCoor fabs(TCoor coor) const
        {
            return coor >= 0 ? coor : -coor;
        }

        // 矩形范围的四个坐标值
        TCoor _xmin;
        TCoor _ymin;
        TCoor _xmax;
        TCoor _ymax;

        double dpresicion;
    };

    // Effect: - 四叉树节点
    // TCoor : - 坐标的数据类型，如int、float、double等数值型
    // TData : - 节点携带的数据的类型
    // Brief : - 
    template<typename TCoor, typename TData, int precision>
    class CQTreeNode
    {
    public:

        // 矩形范围
        typedef CRect<TCoor, precision> CRect;

        explicit CQTreeNode(CRect rect, ushort depth)
            : _rect(rect)
            , _lt(NULL)
            , _rt(NULL)
            , _lb(NULL)
            , _rb(NULL)
            , _depth(depth)
        {
        }


        explicit CQTreeNode(CRect rect, ushort depth, vector<TData*> datas)
            : CQTreeNode(rect, depth)
            , _datas(datas)
        {
        }

        ~CQTreeNode()
        {
            clear();
        }

        // Effect: - 判断指定的点位是否在节点内部
        // In    : - 
        // In    : - 
        // Brief : - 
        bool IsCoorInNode(TCoor x, TCoor y) const
        {
            return _rect.Inter(x, y);
        }

        // Effect: - 判断节点是否有数据
        // In    : - 
        // In    : - 
        // Out   : - 
        // Rtn   : - 
        // Brief : - 
        bool empty() const
        {
            if (!_datas.empty())
            {
                return false;
            }

            if (_lt != NULL && !_lt->empty()) return false;
            if (_rt != NULL && !_rt->empty()) return false;
            if (_lb != NULL && !_lb->empty()) return false;
            if (_rb != NULL && !_rb->empty()) return false;

            return true;
        }

        // Effect: - 清空节点
        // In    : - 
        // In    : - 
        // Out   : - 
        // Rtn   : - 
        // Brief : - 
        void clear()
        {
            clearData();
            deleteMemory(_lt);
            deleteMemory(_rt);
            deleteMemory(_lb);
            deleteMemory(_rb);

        }

        void clearData()
        {
            for (size_t i = 0; i < _datas.size(); i++)
            {
                deleteMemory(_datas[i]);
            }

            _datas.clear();
        }

        // Effect: - 获得当前节点所拥有的数据个数
        // Rtn   : - 
        // Brief : - 
        size_t size() const
        {     
            size_t sz = _datas.size();
            if (_lt) sz += _lt->size();
            if (_rt) sz += _rt->size();
            if (_lb) sz += _lb->size();
            if (_rb) sz += _rb->size();

            return sz;
        }

        // Effect: - 释放一个节点
        // In    : - 
        // Brief : - 
        template<typename T>
        void deleteMemory(T*& node)
        {
            if (node) 
            {
                delete node;
                node = NULL;
            }
        }

        // 节点的数据范围，所有子节点的范围的并集
        CRect _rect;    

        // 四个子节点
        CQTreeNode* _lt;    // 左上
        CQTreeNode* _rt;    // 右上
        CQTreeNode* _lb;    // 左下
        CQTreeNode* _rb;    // 右下

        vector<TData*> _datas; // 节点数据
        ushort _depth;      // 节点的深度
    };


    // Effect: - 四叉树数据基类
    // TCoor : - 坐标的数据类型，如int、float、double等数值型
    // Brief : - 定义四叉树数据必须实现的接口
    template<typename TCoor>
    class CQTreeDataBase
    {
    public:
        explicit CQTreeDataBase() { }

        virtual ~CQTreeDataBase() { }

        // 获取X坐标
        virtual TCoor X() const = 0;

        // 获取Y坐标
        virtual TCoor Y() const = 0;
    };

    // Effect: - 四叉树基类
    // TCoor : - 坐标的类型，例如int,float,double等
    // TData : - 数据的类型，必须实现CQTreeDataBase所指定的接口
    // int precision : - 坐标比较的精度的倒数，如1E-6,则传入1E6
    // Brief : - int precision使用精度的倒数的原因是浮点型不能作为模板参数
    template<typename TCoor, typename TData, int precision = 1E3>
    class CQTreeBase
    {
    public:

        // 矩形范围
        typedef CRect<TCoor, precision> CRect;

        // 四叉树节点
        typedef CQTreeNode<TCoor, TData, precision> CQTreeNode;

        // Effect: - 构造一个四叉树对象
        // In    : - 整体的数据范围，在该范围之外的数据，无法加入到四叉树中
        // In    : - 四叉树的最大深度，深度达到最大值的节点，将停止拆分
        // In    : - 每一个四叉树节点可以携带的最大数据点位个数，
        //           当节点携带的数据点位个数超过该值时，节点将自动拆分
        //           该值必须大于0
        // Brief : - 
        CQTreeBase(const CRect& rect, ushort maxDepth = 10, ulong maxDataNum = 100)
            : _root(NULL)
            , _maxDepth(maxDepth)
            , _maxDataNum(maxDataNum)
        {
            // 至少要能包含一个数据
            if (_maxDataNum < 1)
            {
                _maxDataNum = 1;
            }

            _root = new CQTreeNode(rect, 0);
        }

        virtual ~CQTreeBase()
        {
            if (_root) delete _root;
        }

        // Effect: - 矩形检索
        // In    : - 坐标范围
        // In    : - 
        // In    : - 
        // In    : - 
        // Out   : - 检索结果
        // Rtn   : - 矩形内部或者矩形边上的点
        // Brief : - 数据量很大的时候，不推荐调用该函数获取结果
        bool Search(TCoor xmin, TCoor ymin, TCoor xmax, TCoor ymax, vector<TData*>& hitedDatas) const
        {
            hitedDatas.clear();

            CRect rect(xmin, ymin, xmax, ymax);

            vector<CQTreeNode*> hitedNode;
            Search(xmin, ymin, xmax, ymax, hitedNode);
            if (!hitedNode.empty())
            {
                // 此处可利用Reduce思想进行多线程优化
                for (size_t i = 0; i < hitedNode.size(); i++)
                {
                    vector<TData*> & data = hitedNode[i]->_datas;

                    for (size_t j = 0; j < data.size(); j++)
                    {
                        if( rect.Inter(data[j]->X(), data[j]->Y()) )
                        {
                            hitedDatas.push_back(data[j]);
                        }
                    }
                }
            }

            return !hitedDatas.empty();
        }

        // Effect: - 矩形检索
        // In    : - 坐标范围
        // In    : - 
        // In    : - 
        // In    : - 
        // Out   : - 检索结果
        // Rtn   : - 与矩形碰撞的节点
        // Brief : - 
        bool Search(TCoor xmin, TCoor ymin, TCoor xmax, TCoor ymax, vector<CQTreeNode*>& hitedNode) const
        {
            hitedNode.clear();
            return SearchInNode(_root, xmin, ymin, xmax, ymax, hitedNode);
        }

        // Effect: - 获取所有四叉树叶子节点
        // In    : - 
        // In    : - 
        // Out   : - 
        // Rtn   : - 
        // Brief : - 
        bool SearchAllNode(vector<CQTreeNode*>& hitedNode) const
        {
            hitedNode.clear();

            return SearchInNode(_root, _root->_rect._xmin, _root->_rect._ymin, _root->_rect._xmax, _root->_rect._ymax, hitedNode);
        }

        // Effect: - 获取四叉树所有数据
        // In    : - 
        // In    : - 
        // Out   : - 
        // Rtn   : - 
        // Brief : - 
        bool SearchAllNode(vector<TData*>& allDatas) const
        {
            allDatas.clear();

            vector<CQTreeNode*> allNode;
            SearchAllNode(allNode);

            if (!allNode.empty())
            {
                for (size_t i = 0; i < allNode.size(); i++)
                {
                    allDatas.insert(allDatas.end(), allNode[i]->_datas.begin(), allNode[i]->_datas.end());
                }
            }

            return !allDatas.empty();
        }

        // Effect: - 点检索
        // In    : - 
        // In    : - 
        // Rtn   : - 该点所在的节点
        // Brief : - 
        CQTreeNode* Search(TCoor x, TCoor y) const
        {
            return SearchInNode(_root, x, y);
        }

        // Effect: - 在指定节点进行点检索
        // In    : - 
        // In    : - 
        // Out   : - 
        // Rtn   : - 
        // Brief : - 
        CQTreeNode* Search(CQTreeNode* node, TCoor x, TCoor y) const
        {
            return SearchInNode(node, x, y);
        }

        // Effect: - 点检索
        // In    : - 
        // In    : - 
        // Rtn   : - 该点所在的节点的第一个数据
        // Brief : - 
        TData* SearchData(TCoor x, TCoor y) const
        {
            CQTreeNode* node = Search(x, y);
            if (node != NULL)
            {
                return SearchDataInNode(node, x, y);
            }

            return NULL;
        }

        // Effect: - 在指定节点进行点检索
        // In    : - 
        // In    : - 
        // Rtn   : - 该点所在的节点的第一个数据
        // Brief : - 
        TData* SearchData(CQTreeNode* node, TCoor x, TCoor y) const
        {
            CQTreeNode* nodeSearched = Search(node, x, y);
            if (nodeSearched != NULL)
            {
                return SearchDataInNode(nodeSearched, x, y);
            }

            return NULL;
        }

        // Effect: - 在指定的节点上进行数据检索
        // In    : - 
        // In    : - 
        // Out   : - 
        // Rtn   : - 
        // Brief : - 仅检索指定的节点，不进行递归
        TData* SearchDataInNode(CQTreeNode* node, TCoor x, TCoor y) const
        {
            if (node != NULL)
            {
                vector<TData*> & data = node->_datas;

                CRect rc;

                for (size_t j = 0; j < data.size(); j++)
                {
                    if ( rc.Equal(x, data[j]->X()) && rc.Equal(y, data[j]->Y()) )
                    {
                        return data[j];
                    }
                }
            }

            return NULL;
        }



        // Effect: - 添加一个数据点到树中
        // In    : - 数据
        // Rtn   : - 
        // Brief : - 
        bool AddData(TData* data)
        {
            if (data != NULL)
            {
                return AddDataInNode(Search(data->X(), data->Y()), data);
            }

            return false;
        }

        bool empty(void) const
        {
            return _root->empty();
        }

        size_t size() const
        {
            return _root->size();
        }

        void clear()
        {
            _root->clear();
        }

    protected:

        // Effect: - 矩形检索
        // In    : - 检索起始节点
        // In    : - 坐标范围
        // In    : - 
        // In    : - 
        // In    : - 
        // Out   : - 与指定范围相交的节点集合
        // Rtn   : - 
        // Brief : - 递归进行检索
        virtual bool SearchInNode(CQTreeNode* node, TCoor xmin, TCoor ymin, TCoor xmax, TCoor ymax, vector<CQTreeNode*>& hitedNode) const
        {
            if (node != NULL)
            {
                CRect rc(xmin, ymin, xmax, ymax);

                if (rc.Inter(node->_rect))
                {
                    if (!node->_datas.empty())
                    {
                        hitedNode.push_back(node);
                    }                       
                    else
                    {
                        SearchInNode(node->_lt, xmin, ymin, xmax, ymax, hitedNode);
                        SearchInNode(node->_rt, xmin, ymin, xmax, ymax, hitedNode);
                        SearchInNode(node->_lb, xmin, ymin, xmax, ymax, hitedNode);
                        SearchInNode(node->_rb, xmin, ymin, xmax, ymax, hitedNode);
                    }

                    return true;
                }
            }

            return false;
        }

        // Effect: - 点检索
        // In    : - 开始检索的节点
        // In    : - 
        // In    : - 
        // Rtn   : - 该点所在的节点
        // Brief : - 
        virtual CQTreeNode* SearchInNode(CQTreeNode* node, TCoor x, TCoor y) const
        {
            if (node != NULL && node->IsCoorInNode(x, y))
            {
                if (node->_lt != NULL && node->_lt->IsCoorInNode(x, y))
                {
                    return SearchInNode(node->_lt, x, y);
                }
                else if (node->_rt != NULL && node->_rt->IsCoorInNode(x, y))
                {
                    return SearchInNode(node->_rt, x, y);
                }
                else if (node->_lb != NULL && node->_lb->IsCoorInNode(x, y))
                {
                    return SearchInNode(node->_lb, x, y);
                }
                else if (node->_rb != NULL && node->_rb->IsCoorInNode(x, y))
                {
                    return SearchInNode(node->_rb, x, y);
                }
                else
                {
                    return node;
                }
            }

            return NULL;
        }


        // Effect: - 添加一个数据到指定的节点中
        // In    : - 
        // In    : - 
        // Rtn   : - 
        // Brief : - 
        virtual bool AddDataInNode(CQTreeNode* node, TData* data)
        {
            if (data != NULL && node != NULL && node->IsCoorInNode(data->X(), data->Y()))
            {
                node->_datas.push_back(data);
                return SpiltNode(node);
            }

            return false;
        }

        // Effect: - 拆分指定的节点
        // In    : - 
        // Rtn   : - 
        // Brief : - 
        virtual bool SpiltNode(CQTreeNode* node)
        {
            if (node != NULL)
            {
                // 节点的深度达到最大深度或者节点的数据量还不到最大阈值，无需拆分节点
                if (node->_depth >= _maxDepth || node->_datas.size() <= _maxDataNum)
                {
                    return true;
                }

                // 生成四个子节点
                node->_lt = new CQTreeNode(node->_rect.GetLTRect(), node->_depth + 1);
                node->_rt = new CQTreeNode(node->_rect.GetRTRect(), node->_depth + 1);
                node->_lb = new CQTreeNode(node->_rect.GetLBRect(), node->_depth + 1);
                node->_rb = new CQTreeNode(node->_rect.GetRBRect(), node->_depth + 1);

                // 将数据分配给四个子节点
                for (size_t i = 0; i < node->_datas.size(); i++)
                {
                    TData* data = node->_datas[i];
                    if (data != NULL)
                    {
                        CQTreeNode* subNode = SearchInNode(node, data->X(), data->Y());

                        if (subNode != NULL)
                        {
                            subNode->_datas.push_back(data);
                            SpiltNode(subNode);
                        }
                    }
                }

                // 当前节点的数据需要清空
                node->_datas.clear();

                return true;
            }

            return false;
        }

    protected:

        CQTreeNode* _root;  // 根节点

        ushort _maxDepth;   // 树的最大深度
        ulong  _maxDataNum; // 树节点的最大数据数量        
    };
}
