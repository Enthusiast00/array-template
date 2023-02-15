#ifndef ARRAY_H
#define ARRAY_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

#define _ERROR(X)    fprintf(stderr,"Error: %s \nline:%i file:%s \n",X,__LINE__ , __FILE__)
#define _THROW(X)    { _ERROR(X); return false; }

#define _NEW(X,Y)    ( (X*)calloc(Y, sizeof(X)) )
#define _ALLOC(X,Y,Z){ if(X){ delete X; X=NULL; } X=_NEW(Z,Y); if(!X){ _THROW("Bad Alloc"); } }

#define _SWAP(X,Y) { T c=X; X=Y;Y=c; }

template<typename T ,typename T1 = uint32_t >
class Array {

    T1  m_pos;
    T1  m_size;
    T * m_data;
public:
    Array():m_pos(0),m_size(0),m_data(NULL){}

    ~Array(){ Clear(); }

    Array(const Array& x): m_pos(0),m_size(0),m_data(NULL) { Copy(x); }
    Array(const T& val): m_pos(0),m_size(0),m_data(NULL) { PushBack(val); }
    Array(const T* val): m_pos(0),m_size(0),m_data(NULL) { operator =(val); }

    void operator = (const Array& x) { Copy(x); }
    void operator = (const T& val) { Clear(); PushBack(val); }
    void operator =(const T* val){
        if(val){
            Clear();
            T1 len = strlen(val);
            Allocate(len+1);
            m_pos = len;
            for(T1 i(0); i<len; m_data[i] = val[i], i++);
        }
    }

    void operator += (const T* val){
        if(val){
            T1 len = strlen(val);
            if( !Alloc(m_pos+len+1,false,false) ){ _ERROR("Alloc"); }
            for(T1 i(0); i<len;m_data[m_pos+i] = val[i],i++);
            m_pos+=len;
        }
    }

    void operator += (const T& val) {  PushBack(val); }
    void operator += (const Array&  val) { operator+=(val.Data()); }

    Array operator + (const T& val)const { Array out(m_data); out += val; return out; }
    Array operator + (const T* val)const { Array out(m_data); out += val; return out; }
    Array operator + (const Array& val)const  { return operator+(val.m_data); }

    T& operator [](const T1& index){ return m_data[index]; }
    const T& operator [](const T1& index) const { return m_data[index]; }

    void Copy (const Array& x){
        Clear();
        Allocate(x.m_size);
        m_size = x.m_size; m_pos  = x.m_pos;
        for(T1 i=0; i<m_size; m_data[i] = x.m_data[i], i++);
    }

    void Allocate(const T1& size ){ if( !Alloc(size,true,false) ){ _ERROR("Alloc"); } }

    void PushBack(T val, bool buffered = false){
        if( Alloc(m_pos+2,false,buffered) ){
            m_data[m_pos++] = val;
        }else{ _ERROR("Alloc"); }
    }

    void Clear(){
        if(m_data){ delete m_data; }
        m_data = NULL;
        m_size = m_pos = 0;
    }

private:

    T1 Partition(T1 p, T1 r){

        T x = m_data[r];
        T1 i = p;

        for(T1 j=p; j<r; j++){
            if(m_data[j]<=x ){
                _SWAP(m_data[i],m_data[j]);
                i++;
            }
        }
        _SWAP(m_data[i],m_data[r]);
        return i;
    }
    void QuickSortLoop(T1 p, T1 r){
        if(p<r){
            T1 q = Partition(p,r);
            if((q>0) && (p<q-1)){ QuickSortLoop(p,q-1); }
            if((r>0) && (q+1<r)){ QuickSortLoop(q+1,r); }
        }
    }

public:

    void Sort(T1 st, T1 en){
        if( (st>=en) || (en-st)<=1) { return; }
        QuickSortLoop(st,en-1);
    }

    const T* Data() const { return m_data; }
    const T1& Size() const { return m_pos; }

protected:

    bool Alloc(const T1& size ,bool pos ,bool buffered){

        if(size<=m_size){ return true; }

        T1 asize( size + T1(pos) );
        if(!m_data || !m_pos){ _ALLOC(m_data,asize,T); }
        else{
            asize = buffered? m_size*2 : asize;

            T* buf = NULL;
            _ALLOC(buf,asize,T);
            for(T1 i =0; i<m_pos; buf[i] = m_data[i], i++);

            delete m_data;
            m_data = buf;
        }
        if(pos){ m_pos = size; }
        m_size = asize;
        return true;
    }
};

#endif // ARRAY_H
