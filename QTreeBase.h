/******************************************************************** 

Purpose  : ��װ�Ĳ����Ļ���ʵ�� 

ʵ���Ĳ������Զ��������ɰ�������ʵ���Զ����Զ�����
ʵ���Ĳ����ļ������ɰ�������ʵ���Զ������

Version  : 1.0 
*********************************************************************/
#pragma once

#include <vector>

using namespace std;

namespace QTree
{

    typedef unsigned short ushort;
    typedef unsigned long ulong;

    // Effect: - �������귶Χ
    // TCoor : - ������������ͣ���int��float��double����ֵ��
    // precision �������ľ���
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

        // Effect: - �ж����������Ƿ����
        // In    : - 
        // In    : - 
        // Out   : - 
        // Rtn   : - 
        // Brief : - ��������������밴����д����
        virtual bool Equal(TCoor a, TCoor b) const
        {
            return fabs(a - b) < dpresicion;
        }

        // Effect: - ��ȡ���Ͻǲ�ַ�Χ
        // Rtn   : - 
        // Brief : - 
        CRect GetLTRect() const
        {
            return CRect(_xmin, (_ymax + _ymin) / 2, (_xmin +_xmax) / 2, _ymax);
        }

        // Effect: - ��ȡ���Ͻǲ�ַ�Χ
        // Rtn   : - 
        // Brief : - 
        CRect GetRTRect() const
        {
            return CRect((_xmin +_xmax) / 2, (_ymax + _ymin) / 2, _xmax, _ymax);
        }

        // Effect: - ��ȡ���½ǲ�ַ�Χ
        // Rtn   : - 
        // Brief : - 
        CRect GetLBRect() const
        {
            return CRect(_xmin, _ymin, (_xmin +_xmax) / 2,  (_ymax + _ymin) / 2);
        }

        // Effect: - ��ȡ���½ǲ�ַ�Χ
        // Rtn   : - 
        // Brief : - 
        CRect GetRBRect() const
        {
            return CRect((_xmin +_xmax) / 2, _ymin, _xmax,  (_ymax + _ymin) / 2);
        }

        // Effect: - �ж�ָ���ĵ�λ�Ƿ��ھ��з�Χ��
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

        // Effect: - �ж�ָ���ľ��η�Χ�Ƿ�͵�ǰ��Χ�ཻ
        // In    : - 
        // Rtn   : - 
        // Brief : - 
        bool Inter(const CRect& rhs) const
        {
            return 
                LessEqual(_xmax + _xmin - rhs._xmax - rhs._xmin, _xmax - _xmin + rhs._xmax - rhs._xmin) &&
                LessEqual(_ymax + _ymin - rhs._ymax - rhs._ymin, _ymax - _ymin + rhs._ymax - rhs._ymin);
        }

        // Effect: - ȡ����ֵ
        // In    : - 
        // Rtn   : - 
        // Brief : - 
        TCoor fabs(TCoor coor) const
        {
            return coor >= 0 ? coor : -coor;
        }

        // ���η�Χ���ĸ�����ֵ
        TCoor _xmin;
        TCoor _ymin;
        TCoor _xmax;
        TCoor _ymax;

        double dpresicion;
    };

    // Effect: - �Ĳ����ڵ�
    // TCoor : - ������������ͣ���int��float��double����ֵ��
    // TData : - �ڵ�Я�������ݵ�����
    // Brief : - 
    template<typename TCoor, typename TData, int precision>
    class CQTreeNode
    {
    public:

        // ���η�Χ
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

        // Effect: - �ж�ָ���ĵ�λ�Ƿ��ڽڵ��ڲ�
        // In    : - 
        // In    : - 
        // Brief : - 
        bool IsCoorInNode(TCoor x, TCoor y) const
        {
            return _rect.Inter(x, y);
        }

        // Effect: - �жϽڵ��Ƿ�������
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

        // Effect: - ��սڵ�
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

        // Effect: - ��õ�ǰ�ڵ���ӵ�е����ݸ���
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

        // Effect: - �ͷ�һ���ڵ�
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

        // �ڵ�����ݷ�Χ�������ӽڵ�ķ�Χ�Ĳ���
        CRect _rect;    

        // �ĸ��ӽڵ�
        CQTreeNode* _lt;    // ����
        CQTreeNode* _rt;    // ����
        CQTreeNode* _lb;    // ����
        CQTreeNode* _rb;    // ����

        vector<TData*> _datas; // �ڵ�����
        ushort _depth;      // �ڵ�����
    };


    // Effect: - �Ĳ������ݻ���
    // TCoor : - ������������ͣ���int��float��double����ֵ��
    // Brief : - �����Ĳ������ݱ���ʵ�ֵĽӿ�
    template<typename TCoor>
    class CQTreeDataBase
    {
    public:
        explicit CQTreeDataBase() { }

        virtual ~CQTreeDataBase() { }

        // ��ȡX����
        virtual TCoor X() const = 0;

        // ��ȡY����
        virtual TCoor Y() const = 0;
    };

    // Effect: - �Ĳ�������
    // TCoor : - ��������ͣ�����int,float,double��
    // TData : - ���ݵ����ͣ�����ʵ��CQTreeDataBase��ָ���Ľӿ�
    // int precision : - ����Ƚϵľ��ȵĵ�������1E-6,����1E6
    // Brief : - int precisionʹ�þ��ȵĵ�����ԭ���Ǹ����Ͳ�����Ϊģ�����
    template<typename TCoor, typename TData, int precision = 1E3>
    class CQTreeBase
    {
    public:

        // ���η�Χ
        typedef CRect<TCoor, precision> CRect;

        // �Ĳ����ڵ�
        typedef CQTreeNode<TCoor, TData, precision> CQTreeNode;

        // Effect: - ����һ���Ĳ�������
        // In    : - ��������ݷ�Χ���ڸ÷�Χ֮������ݣ��޷����뵽�Ĳ�����
        // In    : - �Ĳ����������ȣ���ȴﵽ���ֵ�Ľڵ㣬��ֹͣ���
        // In    : - ÿһ���Ĳ����ڵ����Я����������ݵ�λ������
        //           ���ڵ�Я�������ݵ�λ����������ֵʱ���ڵ㽫�Զ����
        //           ��ֵ�������0
        // Brief : - 
        CQTreeBase(const CRect& rect, ushort maxDepth = 10, ulong maxDataNum = 100)
            : _root(NULL)
            , _maxDepth(maxDepth)
            , _maxDataNum(maxDataNum)
        {
            // ����Ҫ�ܰ���һ������
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

        // Effect: - ���μ���
        // In    : - ���귶Χ
        // In    : - 
        // In    : - 
        // In    : - 
        // Out   : - �������
        // Rtn   : - �����ڲ����߾��α��ϵĵ�
        // Brief : - �������ܴ��ʱ�򣬲��Ƽ����øú�����ȡ���
        bool Search(TCoor xmin, TCoor ymin, TCoor xmax, TCoor ymax, vector<TData*>& hitedDatas) const
        {
            hitedDatas.clear();

            CRect rect(xmin, ymin, xmax, ymax);

            vector<CQTreeNode*> hitedNode;
            Search(xmin, ymin, xmax, ymax, hitedNode);
            if (!hitedNode.empty())
            {
                // �˴�������Reduce˼����ж��߳��Ż�
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

        // Effect: - ���μ���
        // In    : - ���귶Χ
        // In    : - 
        // In    : - 
        // In    : - 
        // Out   : - �������
        // Rtn   : - �������ײ�Ľڵ�
        // Brief : - 
        bool Search(TCoor xmin, TCoor ymin, TCoor xmax, TCoor ymax, vector<CQTreeNode*>& hitedNode) const
        {
            hitedNode.clear();
            return SearchInNode(_root, xmin, ymin, xmax, ymax, hitedNode);
        }

        // Effect: - ��ȡ�����Ĳ���Ҷ�ӽڵ�
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

        // Effect: - ��ȡ�Ĳ�����������
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

        // Effect: - �����
        // In    : - 
        // In    : - 
        // Rtn   : - �õ����ڵĽڵ�
        // Brief : - 
        CQTreeNode* Search(TCoor x, TCoor y) const
        {
            return SearchInNode(_root, x, y);
        }

        // Effect: - ��ָ���ڵ���е����
        // In    : - 
        // In    : - 
        // Out   : - 
        // Rtn   : - 
        // Brief : - 
        CQTreeNode* Search(CQTreeNode* node, TCoor x, TCoor y) const
        {
            return SearchInNode(node, x, y);
        }

        // Effect: - �����
        // In    : - 
        // In    : - 
        // Rtn   : - �õ����ڵĽڵ�ĵ�һ������
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

        // Effect: - ��ָ���ڵ���е����
        // In    : - 
        // In    : - 
        // Rtn   : - �õ����ڵĽڵ�ĵ�һ������
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

        // Effect: - ��ָ���Ľڵ��Ͻ������ݼ���
        // In    : - 
        // In    : - 
        // Out   : - 
        // Rtn   : - 
        // Brief : - ������ָ���Ľڵ㣬�����еݹ�
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



        // Effect: - ���һ�����ݵ㵽����
        // In    : - ����
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

        // Effect: - ���μ���
        // In    : - ������ʼ�ڵ�
        // In    : - ���귶Χ
        // In    : - 
        // In    : - 
        // In    : - 
        // Out   : - ��ָ����Χ�ཻ�Ľڵ㼯��
        // Rtn   : - 
        // Brief : - �ݹ���м���
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

        // Effect: - �����
        // In    : - ��ʼ�����Ľڵ�
        // In    : - 
        // In    : - 
        // Rtn   : - �õ����ڵĽڵ�
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


        // Effect: - ���һ�����ݵ�ָ���Ľڵ���
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

        // Effect: - ���ָ���Ľڵ�
        // In    : - 
        // Rtn   : - 
        // Brief : - 
        virtual bool SpiltNode(CQTreeNode* node)
        {
            if (node != NULL)
            {
                // �ڵ����ȴﵽ�����Ȼ��߽ڵ�������������������ֵ�������ֽڵ�
                if (node->_depth >= _maxDepth || node->_datas.size() <= _maxDataNum)
                {
                    return true;
                }

                // �����ĸ��ӽڵ�
                node->_lt = new CQTreeNode(node->_rect.GetLTRect(), node->_depth + 1);
                node->_rt = new CQTreeNode(node->_rect.GetRTRect(), node->_depth + 1);
                node->_lb = new CQTreeNode(node->_rect.GetLBRect(), node->_depth + 1);
                node->_rb = new CQTreeNode(node->_rect.GetRBRect(), node->_depth + 1);

                // �����ݷ�����ĸ��ӽڵ�
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

                // ��ǰ�ڵ��������Ҫ���
                node->_datas.clear();

                return true;
            }

            return false;
        }

    protected:

        CQTreeNode* _root;  // ���ڵ�

        ushort _maxDepth;   // ����������
        ulong  _maxDataNum; // ���ڵ�������������        
    };
}
