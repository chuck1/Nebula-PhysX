#ifndef PHYSX_CORE_SHAPE_SPHERE_HH
#define PHYSX_CORE_SHAPE_SPHERE_HH

#include <neb/phx/core/shape/base.hpp>

namespace neb { namespace phx { namespace core { namespace shape {
	class sphere:
		virtual public neb::phx::core::shape::base
	{
		public:
			virtual void				init();
			virtual void				release();
			virtual void				step(gal::etc::timestep  const & ts);
			virtual physx::PxGeometry*		to_geo();
	};
}}}}

#endif
