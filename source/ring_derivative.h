
namespace lars{

	template <unsigned axis,class A> typename A::Copy ring_derivative(const A &array,typename A::Scalar min,typename A::Scalar max){
	  typename A::Copy result;
	  result.resize(array.shape());
	  auto s = max - min;  
	  auto c = (max + min)/2;

	  auto ring = [=](typename A::Scalar v)->typename A::Scalar{
	    return v < min ? v+s : v>=max ? v-s : v;
	  };

	  result.for_all_indices([&](typename A::Index idx){ 
	    if(idx.template get<axis>() == 0){
	      auto ip1 = idx;
	      ip1.template set<axis>(idx.template get<axis>() + 1);
	      auto v0 = array(idx);
	      auto vp1 = ring(array(ip1) - v0 + c) - c;
	      result(idx) = vp1;
	    }
	    else if(idx.template get<axis>() == array.shape().template get<axis>() - 1){
	      auto im1 = idx;
	      im1.template set<axis>(idx.template get<axis>() - 1);
	      auto v0 = array(idx);
	      auto vm1 = ring(v0 - array(im1) + c) - c;
	      result(idx) = vm1;
	    }    
	    else{
	      auto im1 = idx, ip1 = idx;
	      im1.template set<axis>(idx.template get<axis>() - 1); 
	      ip1.template set<axis>(idx.template get<axis>() + 1);
	      auto v0 = array(idx);
	      auto vp1 = ring(array(ip1) - v0 + c) - c;
	      auto vm1 = ring(array(im1) - v0 + c) - c;
	      result(idx) = 0.5 * (vp1 - vm1);
	    } 
	  });
	  
	  return result;
	}

}

