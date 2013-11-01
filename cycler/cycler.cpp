#include <cycler.h> 

biInc::biInc(byte mx): _maxVal(mx), _incVal(1) {
  _val = _minVal;  
}
    
byte biInc::getVal() const {
  return _val;
}

void biInc::setVal(byte v)  {
  if (v <= _maxVal && v >= _minVal){
    _val = v;
  }
}

byte biInc::inc(char i){
  return _val = peek(i);
}   

byte biInc::peek(char i) const{
  return  min(max(_val + (i > 0 ? _incVal : -_incVal),
		  _minVal),
	      _maxVal);
}   
  
cycler::cycler(byte mx): biInc(mx){}

byte cycler::inc(){
  return _val = peek();
}
byte cycler::peek() const{
 return  _val == _maxVal ? 0 : _val+1;
}
