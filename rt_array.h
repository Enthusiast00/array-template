#ifndef RT_ARRAY_H
#define RT_ARRAY_H



#define DATA_ALLOC(X,Y){ if(X){ delete X; X=RT_NULL; } X=RT_NEW(T,Y); if(!X){ RT_THROW("Bad Alloc"); } }


template<typename T ,typename T1 = rt_uint32 >
class rt_array {

    T1  m_pos;
    T1  m_size;
    T * m_data;
public:
    rt_array():m_pos(0),m_size(0),m_data(RT_NULL){}

    rt_array(const rt_array& x): m_pos(0),m_size(0),m_data(RT_NULL) { Copy(x); }
    rt_array(const T& val): m_pos(0),m_size(0),m_data(RT_NULL) { PushBack(val); }
    rt_array(const T* val): m_pos(0),m_size(0),m_data(RT_NULL) { operator =(val); }

    void operator = (const rt_array& x) { Copy(x); }

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
            if(!Alloc(m_pos+len+1,false,false) ){ RT_ERROR("Alloc"); }
            for(T1 i(0); i<len;m_data[m_pos+i] = val[i],i++);
            m_pos+=len;
        }
    }

    void operator += (const T& val) {  PushBack(val); }
    void operator += (const rt_array&  val) { operator+=(val.Data()); }

    rt_array operator + (const T& val)const { rt_array out(m_data); out += val; return out; }
    rt_array operator + (const T* val)const { rt_array out(m_data); out += val; return out; }
    rt_array operator + (const rt_array& val)const  { return operator+(val.m_data); }

    T& operator [](const T1& index){ return m_data[index]; }
    const T& operator [](const T1& index) const { return m_data[index]; }

    void Copy (const rt_array& x){
        Clear();
        Allocate(x.m_size);
        m_size = x.m_size; m_pos  = x.m_pos;
        for(T1 i=0; i<m_size; m_data[i] = x.m_data[i], i++);
    }

    void Allocate(const T1& size ){ if( !Alloc(size,true,false) ){ RT_ERROR("Alloc"); } }

    void PushBack(T val, bool buffered = false){
        if( Alloc(m_pos+2,false,buffered) ){
            m_data[m_pos++] = val;
        }else{ RT_ERROR("Alloc"); }
    }

    void Clear(){
        if(m_data){ delete m_data; }
        m_data = RT_NULL;
        m_size = m_pos = 0;
    }

    void Sort(){
        //                InsertionSort((T*)m_data,0,m_pos,true);
        HeapSort((T*)m_data,0,m_pos,true);
        //        QuickSort(m_data,0,m_pos);
    }

    const T* Data() const { return m_data; }
    const T1& Size() const { return m_pos; }



protected:

    bool Alloc(const T1& size ,bool pos ,bool buffered){

        if(size<m_size){ return true; }

        T1 asize(size);
        if(!m_data || !m_pos){ RT_ALLOC(m_data,asize,T); }
        else{
            asize = buffered? m_size*2 : asize;

            T* buf = RT_NULL;
            RT_ALLOC(buf,asize,T);
            for(T1 i =0; i<m_pos; buf[i] = m_data[i], i++);

            delete m_data;
            m_data = buf;
        }
        if(pos){ m_pos = size; }
        m_size = asize;
        return true;
    }
};

#endif // RT_ARRAY_H
