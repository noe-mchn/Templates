#include <iostream>
#include <stdexcept>
#include "ArrayN.h"
#include "VecteurND.h"
#include "MatrixN.h"
#include "VectorN.h"
#include "ListN.h"
#include "IntrusiveListN.h"

static void testVectorN()
{
    std::cout << "=== Test VectorN ===" << std::endl;

    //----- Test constructor of the VectorN ------
    VectorN<int> ConstructVector1;
    if (ConstructVector1.size() != 0)
        throw std::runtime_error("Default constructor error");

    VectorN<int> ConstructVector2(5, 42);
    if (ConstructVector2.size() != 5 || ConstructVector2[0] != 42 || ConstructVector2[4] != 42)
        throw std::runtime_error("Constructor error");

    VectorN<int> ConstructVector3 = { 1, 2, 3 };
    if (ConstructVector3.size() != 3 || ConstructVector3[0] != 1 || ConstructVector3[1] != 2 || ConstructVector3[2] != 3)
        throw std::runtime_error("Initializer list constructor error");

    VectorN<int> ConstructVector4(ConstructVector3);
    if (ConstructVector4.size() != 3 || ConstructVector4[0] != 1 || ConstructVector4[1] != 2 || ConstructVector4[2] != 3)
        throw std::runtime_error("Copy constructor error");

    VectorN<int> ConstructVector5;
    ConstructVector5 = ConstructVector2;
    if (ConstructVector5.size() != 5 || ConstructVector5[0] != 42 || ConstructVector5[4] != 42)
        throw std::runtime_error("Copy assignment error");


    // ------ Test push and pop back ------
    VectorN<int> vector;
    vector.push_back(10);
    vector.push_back(20);
    vector.push_back(30);

    if (vector.size() != 3 || vector[0] != 10 || vector[1] != 20 || vector[2] != 30)
        throw std::runtime_error("push_back error");

    vector.pop_back();
    if (vector.size() != 2 || vector[0] != 10 || vector[1] != 20)
        throw std::runtime_error("pop_back error");

    vector.pop_back();
    if (vector.size() != 1 || vector[0] != 10)
        throw std::runtime_error("pop_back error");

    vector.pop_back();
    if (!vector.empty())
        throw std::runtime_error("pop_back error");


    // ------ Test push and pop front ------
    VectorN<int> v;
    v.push_front(1);
    v.push_front(2);
    v.push_front(3);
    if (v.size() != 3 || v[0] != 3 || v[1] != 2 || v[2] != 1)
        throw std::runtime_error("push_front error");

    v.pop_front();
    if (v.size() != 2 || v[0] != 2 || v[1] != 1)
        throw std::runtime_error("pop_front error");

    v.pop_front();
    if (v.size() != 1 || v[0] != 1)
        throw std::runtime_error("pop_front error");

    v.pop_front();
    if (!v.empty())
        throw std::runtime_error("pop_front error");


    // ------ Test at() and operator[] ------
    VectorN<int> v1 = { 10, 20, 30 };

    if (v1[0] != 10 || v1[2] != 30)
        throw std::runtime_error("operator[] error");

    if (v1.at(1) != 20)
        throw std::runtime_error("at error");

    bool caught = false;
    try
    {
        v1.at(3);
    }
    catch (const std::out_of_range&)
    {
        caught = true;
    }
    if (!caught)
        throw std::runtime_error("invalid index");


    // ------ Test insert / emplace / erase ------
    VectorN<std::string> vstring = { "Hello", "World", "!" };

    auto itPos = vstring.insert(vstring.begin() + 1, "Inserted");
    if (*itPos != "Inserted")
        throw std::runtime_error("insert returned wrong iterator");

    if (vstring.size() != 4 || vstring[1] != "Inserted" || vstring[2] != "World")
        throw std::runtime_error("insert error");

    auto itErase = vstring.erase(vstring.begin() + 2);
    if (*itErase != "!")
        throw std::runtime_error("erase returned wrong iterator");
    if (vstring.size() != 3 || vstring[2] != "!")
        throw std::runtime_error("erase error");

    VectorN<std::pair<int, int>> vPair = { {1,2}, {3,4} };
    vPair.emplace(vPair.begin() + 1, 5, 6);
    if (vPair.size() != 3 || vPair[1].first != 5 || vPair[1].second != 6)
        throw std::runtime_error("emplace error");

    vPair.erase(vPair.begin() + 2);
    if (vPair.size() != 2)
        throw std::runtime_error("erase error");


    // ------ Test resize / reserve ------
    VectorN<int> v2;
    v2.reserve(10);
    if (v2.capacity() < 10)
        throw std::runtime_error("reserve error");

    if (v2.size() != 0)
        throw std::runtime_error("reserve changed size unexpectedly");

    v2.resize(5, 99);
    if (v2.size() != 5 || v2.capacity() < 5)
        throw std::runtime_error("resize error");

    for (std::size_t i = 0; i < v2.size(); ++i)
    {
        if (v2[i] != 99)
            throw std::runtime_error("resize with fill value error");
    }

    v2.resize(2);
    if (v2.size() != 2)
        throw std::runtime_error("resize error");

    if (v2.capacity() < 5)
        throw std::runtime_error("capacity error");


    // ------ Test assign / assign_range ------
    VectorN<char> v3;
    v3.assign(5, 'a');
    if (v3.size() != 5 || v3[0] != 'a' || v3[4] != 'a')
        throw std::runtime_error("assign error");

    std::string s = "Hello";
    VectorN<char> v4;
    v4.assign_range(s.begin(), s.end());
    if (v4.size() != 5 || v4[0] != 'H' || v4[4] != 'o')
        throw std::runtime_error("assign_range error");


    // ------ Test clear ------
    VectorN<int> v5 = { 1,2,3 };
    v5.clear();
    if (v5.size() != 0)
        throw std::runtime_error("clear did not set size to 0");

    if (!v5.empty())
        throw std::runtime_error("empty not true after clear");


    // ------ Test swap ------
    VectorN<int> v6 = { 1,2,3 };
    VectorN<int> v7 = { 10,20 };

    v6.swap(v7);

    if (v6.size() != 2 || v6[0] != 10 || v6[1] != 20)
        throw std::runtime_error("swap error");

    if (v7.size() != 3 || v7[0] != 1 || v7[2] != 3)
        throw std::runtime_error("swap error");

    std::cout << "VectorN test passed!" << std::endl;

}



static void testListN()
{
    std::cout << "\n=== Test ListN ===" << std::endl;

    //------Test constructors------
    ListN<int> ConstructorList1;
    if (!ConstructorList1.empty())
        throw std::runtime_error("Default constructor failed");

    ListN<int> ConstructorList2 = { 1, 2, 3 };
    if (ConstructorList2.size() != 3 || ConstructorList2.front() != 1 || ConstructorList2.back() != 3)
        throw std::runtime_error("Initializer list constructor failed.");

    ListN<int> ConstructorList3(ConstructorList2);
    if (ConstructorList3.size() != 3 || ConstructorList3.front() != 1 || ConstructorList3.back() != 3)
        throw std::runtime_error("Copy constructor failed.");

    ListN<int> ConstructorList4;
    ConstructorList4 = ConstructorList3;
    if (ConstructorList4.size() != 3 || ConstructorList4.front() != 1 || ConstructorList4.back() != 3)
        throw std::runtime_error("Copy assignment failed.");


    //------Test push/pop front/back------
    ListN<int> list1;
    list1.push_front(10);
    list1.push_front(20);
    list1.push_front(30);
    if (list1.size() != 3 || list1.front() != 30 || list1.back() != 10)
        throw std::runtime_error("push_front failed.");

    list1.push_back(40);
    if (list1.size() != 4 || list1.back() != 40)
        throw std::runtime_error("push_back failed.");

    list1.pop_front();
    if (list1.size() != 3 || list1.front() != 20)
        throw std::runtime_error("pop_front failed.");

    list1.pop_back();
    if (list1.size() != 2 || list1.back() != 10)
        throw std::runtime_error("pop_back failed.");


    //------Test Iterators------
    ListN<int> list2 = { 1, 2, 3, 4, 5 };

    int expected = 1;
    for (auto it = list2.begin(); it != list2.end(); ++it)
    {
        if (*it != expected)
            throw std::runtime_error("Iterator iteration mismatch.");
        expected++;
    }

    const ListN<int> constList = list2;
    expected = 1;
    for (auto cit = constList.begin(); cit != constList.end(); ++cit)
    {
        if (*cit != expected)
            throw std::runtime_error("const_iterator mismatch.");
        expected++;
    }


    //------Test insert/erase------
    ListN<std::string> ListString = { "A", "B" };

    auto itPos = ListString.insert(ListString.begin(), "A");
    if (*itPos != "A")
        throw std::runtime_error("insert returned wrong iterator.");

    if (ListString.size() != 3 || ListString.front() != "A")
        throw std::runtime_error("insert at begin failed.");

    itPos = ListString.begin();
    ++itPos;
    itPos = ListString.erase(itPos);
    if (*itPos != "B")
        throw std::runtime_error("erase returned wrong iterator.");


    //------Test resize------
    ListN<int> list3 = { 1, 2, 3 };
    list3.resize(5, 99);
    if (list3.size() != 5 || list3.back() != 99)
        throw std::runtime_error("resize increase failed");

    list3.resize(2);
    if (list3.size() != 2 || list3.back() != 2)
        throw std::runtime_error("resize decrease failed");


    //------Test assign/assign_range------
    ListN<char> Charlist;
    Charlist.assign(5, 'a');
    if (Charlist.size() != 5 || Charlist.front() != 'a' || Charlist.back() != 'a')
        throw std::runtime_error("assign failed");

    std::string s = "Hello";
    ListN<char> Charlist2;
    Charlist2.assign_range(s.begin(), s.end());
    if (Charlist2.size() != 5 || Charlist2.front() != 'H' || Charlist2.back() != 'o')
        throw std::runtime_error("assign_range failed");


    //------Test swap------
    ListN<int> SwapList1 = { 1,2,3 };
    ListN<int> ListSwap2 = { 10, 20 };
    SwapList1.swap(ListSwap2);

    if (SwapList1.size() != 2 || SwapList1.front() != 10 || SwapList1.back() != 20)
        throw std::runtime_error("swap error");

    if (ListSwap2.size() != 3 || ListSwap2.front() != 1 || ListSwap2.back() != 3)
        throw std::runtime_error("swap error");


    //------Test clear------
    ListN<int> listClear = { 1,2,3 };
    listClear.clear();
    if (!listClear.empty())
        throw std::runtime_error("clear failed");


    std::cout << "ListN test passed!" << std::endl;
}




static void testIntrusiveListN()
{
    std::cout << "\n=== Test IntrusiveListN ===" << std::endl;

    //------ Test empty ------
    {
        IntrusiveList<Node, &Node::hook> listEmpty;
        if (!listEmpty.empty() || listEmpty.size() != 0)
            throw std::runtime_error("List should be empty initially");
    }


    //------ Test push front / push back ------
    {
        IntrusiveList<Node, &Node::hook> list;
        Node n1(1), n2(2), n3(3);

        list.push_back(n1);
        list.push_front(n2);
        list.push_back(n3);

        if (list.size() != 3)
            throw std::runtime_error("push_front/push_back size error");

        if (list.front().data != 2 || list.back().data != 3)
            throw std::runtime_error("front / back error");

        list.pop_front();
        if (list.front().data != 1 || list.back().data != 3 || list.size() != 2)
            throw std::runtime_error("pop_front error");

        list.pop_back();
        if (list.front().data != 1 || list.back().data != 1 || list.size() != 1)
            throw std::runtime_error("pop_back error");

        list.pop_back();
        if (!list.empty())
            throw std::runtime_error("pop_back last element error");
    }


    //----- Test clear() -----
    {
        IntrusiveList<Node, &Node::hook> list;
        Node n1(100), n2(200), n3(300);

        list.push_back(n1);
        list.push_back(n2);
        list.push_back(n3);
        list.clear();

        if (!list.empty() || list.size() != 0)
            throw std::runtime_error("clear error");

        if (n1.hook.is_linked() || n2.hook.is_linked() || n3.hook.is_linked())
            throw std::runtime_error("clear error");
    }


    IntrusiveList<Node, &Node::hook> list;
    Node n1(10), n2(20), n3(20), n4(15), n5(5);


    //------ push_front / push_back / insert ------
    list.push_back(n1);
    list.push_back(n2);
    list.push_front(n3);
    list.insert(list.begin(), n4);
    if (list.size() != 4)
        throw std::runtime_error("insert / push_front / push_back error");


    //------ pop_front / pop_back ------
    list.pop_front();
    if (list.front().data != 20)
        throw std::runtime_error("pop_front error");
    list.pop_back();
    if (list.back().data != 10)
        throw std::runtime_error("pop_back error");


    //------ swap ------
    {
        IntrusiveList<Node, &Node::hook> list2;
        Node nx(1), ny(2);
        list2.push_back(nx);
        list2.push_back(ny);

        list.swap(list2);
        if (list.size() != 2 || list2.size() != 2)
            throw std::runtime_error("swap size error");

        if (list.front().data != 1 || list.back().data != 2)
            throw std::runtime_error("swap content error");

        if (list2.front().data != 20 || list2.back().data != 10)
            throw std::runtime_error("swap content error");

        list.swap(list2);
    }


    //------ merge ------
    {
        IntrusiveList<Node, &Node::hook> list3;
        Node n6(100), n7(200);
        list3.push_back(n6);
        list3.push_back(n7);

        list.merge(list3);
        if (list3.size() != 0 || list.size() != 4)
            throw std::runtime_error("merge size error");
    }


    //------ splice (pos, other) ------
    {
        IntrusiveList<Node, &Node::hook> list4;
        Node n8(300), n9(400);
        list4.push_back(n8);
        list4.push_back(n9);

        list.splice(list.end(), list4);

        if (list4.size() != 0 || list.size() != 6)
            throw std::runtime_error("splice error");
    }

    std::cout << "IntrusiveListN test passed!" << std::endl;
}


// Fonction de test pour ArrayN
static void testArrayN()
{
    std::cout << "\n=== Test ArrayN ===" << std::endl;

    ArrayN<int, 5> array;

    for (std::size_t i = 0; i < array.size; ++i)
        array[i] = static_cast<int>(i * 10);

    for (std::size_t i = 0; i < array.size; ++i)
    {
        if (array[i] != static_cast<int>(i * 10))
            throw std::runtime_error("ArrayN test failed: wrong stockage of the values.");
    }

    std::cout << "ArrayN test passed!" << std::endl;
}

// Fonction de test pour VectorND
static void testVectorND()
{
    std::cout << "\n=== Test VectorND ===" << std::endl;

    VectorND<float, 3> vec3 = { 1.0f, 2.0f, 3.0f };
    VectorND<float, 3> vec3b = { 3.0f, 2.0f, 1.0f };

    if (vec3.size() != 3)
        throw std::runtime_error("VectorND test failed: size wrong");


    float dotResult = VectorND<float, 3>::dot(vec3, vec3b);
    if (dotResult != 10.0f)
        throw std::runtime_error("VectorND test failed: scalar product incorrect");


    auto crossResult = VectorND<float, 3>::cross(vec3, vec3b);
    if (crossResult[0] != -4.0f || crossResult[1] != 8.0f || crossResult[2] != -4.0f)
        throw std::runtime_error("VectorND test failed: dot product incorrect");

    // Test norm
    float normVal = vec3.norm();
    if (std::abs(normVal - 3.741657386f) > 1e-5f)
        throw std::runtime_error("VectorND test failed: norme incorrecte");

    // Test normalize
    vec3.normalize();
    if (std::abs(vec3.norm() - 1.0f) > 1e-5f)
        throw std::runtime_error("VectorND test failed: normalization incorrecte");

    std::cout << "VectorND test passed!" << std::endl;
}

// Fonction de test pour MatrixND
static void testMatrixND()
{
    std::cout << "\n=== Test MatrixND ===" << std::endl;


    MatrixND<int, 2, 3> matA{ {
        {1, 2, 3},
        {4, 5, 6}
    } };


    MatrixND<int, 3, 2> matB{ {
        {7,  8},
        {9,  10},
        {11, 12}
    } };


    auto matC = MatrixND<int, 2, 3>::multiply<2>(matA, matB);

    if (matC(0, 0) != 58 || matC(0, 1) != 64 ||
        matC(1, 0) != 139 || matC(1, 1) != 154)
    {
        throw std::runtime_error("MatrixND test failed: wrong multiplication incorrecte");
    }

    std::cout << "MatrixND test passed!" << std::endl;
}


int Test()
{
    try
    {
        testVectorN();
        testListN();
        testIntrusiveListN();
        testArrayN();
        testVectorND();
        testMatrixND();
        std::cout << "\n=== ALL TESTS PASSED ===" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Test failed : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}