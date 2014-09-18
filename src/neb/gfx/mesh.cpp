#include <iostream>
#include <iomanip>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <glm/gtx/transform.hpp>

#include <gal/log/log.hpp>

#include <neb/core/math/geo/polyhedron.hh>
#include <neb/core/math/geo/polygon.hpp>
#include <neb/core/util/debug.hpp>

#include <PxPhysicsAPI.h>

#include <neb/gfx/free.hpp>
#include <neb/gfx/core/mesh.hh>
#include <neb/gfx/texture.hpp>
#include <neb/gfx/util/log.hpp>
#include <neb/gfx/glsl/program/threed.hpp>
#include <neb/gfx/glsl/attrib.hh>
#include <neb/gfx/glsl/uniform/scalar.hpp>
#include <neb/gfx/glsl/buffer/mesh.hpp>
#include <neb/gfx/app/__gfx_glsl.hpp>

neb::gfx::mesh::tri1::tri1() {
	//printf("%s\n",__PRETTY_FUNCTION__);
}
neb::gfx::mesh::tri1::~tri1() {
	//printf("%s\n",__PRETTY_FUNCTION__);
}
void	neb::gfx::mesh::tri1::construct(math::geo::polyhedron* poly) {

	std::cout << __PRETTY_FUNCTION__ << this << std::endl;

	assert(poly);

	math::geo::triangle* tris = poly->getTriangles();
	unsigned int nbTriangles = poly->getNbTriangles();
	
	assert(tris);

	LOG(lg, neb::gfx::sl, debug) << __PRETTY_FUNCTION__;
	LOG(lg, neb::gfx::sl, debug) << "tris: " << nbTriangles;


	nbVerts_   = 3 * nbTriangles;
	nbIndices_ = 3 * nbTriangles;
	vertices_  = new math::geo::vertex[nbVerts_];
	indices_   = new index_type[nbIndices_];
		
	for(size_t i = 0; i < nbIndices_; ++i) {
		indices_[i] = i;
	}
	
	
	for(unsigned int i = 0; i < nbTriangles; i++)
	{
		for(unsigned int j = 0; j < 3; j++)
		{
			vertices_[i*3 + j] = *(tris[i].verts_[j]);
		}
	}

	LOG(lg, neb::gfx::sl, debug) << "vertices: " << nbVerts_;
	LOG(lg, neb::gfx::sl, debug) << "indices:  " << nbIndices_;


}
void			neb::gfx::mesh::tri1::setVerts(math::geo::vertex* verts, unsigned int nbVerts)
{
	vertices_ = verts;
	nbVerts_ = nbVerts;
}
void			neb::gfx::mesh::tri1::setIndices(index_type* indices, unsigned int nbIndices)
{
	indices_ = indices;
	nbIndices_ = nbIndices;
}
unsigned int		neb::gfx::mesh::tri1::getNbIndices()
{
	return nbIndices_;
}
void		neb::gfx::mesh::tri1::serialize(boost::archive::polymorphic_iarchive & ar, unsigned int const & version) {
	
	printf("%s\n",__PRETTY_FUNCTION__);
	
	/*std::string filename = std::string(GLUTPP_OBJECT_DIR) + "/" + name;
	
	FILE * fp;
	
	std::cout << "load file " << filename << std::endl;

	fp = fopen(filename.c_str(), "rb");

	if (fp <= 0) {
		perror("fopen");
		abort();
	}*/
	
	//ar & vertices_;
	//ar & indices_;
	
	
	LOG(lg, neb::gfx::sl, info) << "vertices: " << nbVerts_;
	LOG(lg, neb::gfx::sl, info) << "indices:  " << nbIndices_;

}
void		neb::gfx::mesh::tri1::serialize(boost::archive::polymorphic_oarchive & ar, unsigned int const & version) {

	printf("%s\n",__PRETTY_FUNCTION__);
	
	//ar & vertices_;
	//ar & indices_;
	
	LOG(lg, neb::gfx::sl, info) << "vertices: " << nbVerts_;
	LOG(lg, neb::gfx::sl, info) << "indices:  " << nbIndices_;

}
void		neb::gfx::mesh::tri1::print(int sl) {
	LOG(lg, neb::gfx::sl, (severity_level)sl) << "mesh";

/*	for(auto v : vertices_) {
		v.print(sl);
	}	*/
}
/*
void			neb::gfx::mesh::tri1::init_buffer(
		program_shared p)
{
	LOG(lg, neb::gfx::sl, debug) << __PRETTY_FUNCTION__;

	//glEnable(GL_TEXTURE_2D);

	if(nbIndices_ == 0) {
		printf("not initialized\n");
		assert(0);
		// return instead of abort so shape::base objects for making lights can pass through
		/// @todo need better system for this (like seperate mesh objects that implement this function!!
		return;
	}

	checkerror("unknown");

	auto buf(std::make_shared<neb::gfx::glsl::buffer::tri1>());
	buffers_[p.get()] = buf;

	buf->init(p);

	bufferData(buf);
}
*/
void			neb::gfx::mesh::tri1::drawElements(
		neb::gfx::glsl::program::base const * const & p,
		neb::core::pose const & pose,
		glm::vec3 scale)
{
	LOG(lg, neb::gfx::sl, info) << __PRETTY_FUNCTION__;
	assert(p);

	p->use();

	int tex_num = 3;

	if(normal_map_) {
		LOG(lg, neb::gfx::sl, info) << "activate normal map";

		glActiveTexture(GL_TEXTURE0 + tex_num);
		checkerror("glActiveTexture");

		if(1) {
			// perhaps needs to be generated by this program
			normal_map_->init_buffer(0);

			// here is the culprit, causes 'invalid op' at glDrawElements
			// perhaps something to do wth texture target types or whatever...
			normal_map_->bind(p);
		}
		neb::gfx::ogl::glUniform(p->uniform_table_[neb::gfx::glsl::uniforms::TEX_NORMAL_MAP], tex_num);

		neb::gfx::ogl::glUniform(p->uniform_table_[neb::gfx::glsl::uniforms::HAS_NORMAL_MAP], true);
	} else {
		LOG(lg, neb::gfx::sl, info) << "do not activate normal map";
		neb::gfx::ogl::glUniform(p->uniform_table_[neb::gfx::glsl::uniforms::HAS_NORMAL_MAP], false);
	}

	checkerror("unknown");

	base_t::drawElements(p, pose, scale, nbIndices_);


	/*	// initialize buffers if not already
		if(!buffers_[p.get()])
		{	
		init_buffer(p);
		}
		auto buf = buffers_[p.get()];

		if(!buf) return;

		mesh_base::drawElements(p, buf, pose, scale);
		*/

	/*
	// Uniforms
	// ========
	// material
	material_front_.load(p);

	// texture
	if(normal_map_) {
	LOG(lg, neb::gfx::sl, debug) << "activate normal map";
	glActiveTexture(GL_TEXTURE0);//checkerror("glActiveTexture");
	normal_map_->bind(context);
	p->get_uniform_scalar("normal_map")->load(0);

	p->get_uniform_scalar("has_normal_map")->load_b(true);
	} else {
	p->get_uniform_scalar("has_normal_map")->load_b(false);
	}

	if(texture_) {
	LOG(lg, neb::gfx::sl, debug) << "activate texture";
	glActiveTexture(GL_TEXTURE1);//checkerror("glActiveTexture");
	texture_->bind(context);
	p->get_uniform_scalar("texture")->load(1);

	p->get_uniform_scalar("has_texture")->load_b(true);
	} else {
	p->get_uniform_scalar("has_texture")->load_b(false);
	}
	 *
	 */

}
void			neb::gfx::mesh::tri1::drawDebug(
		neb::gfx::glsl::program::base const * const & p,
		neb::core::pose const & pose,
		glm::vec3 scale)
{
	LOG(lg, neb::gfx::sl, info) << __PRETTY_FUNCTION__;
	assert(p);

	// model matrix
	LOG(lg, neb::gfx::sl, debug) << "load modelview matrix";
	glm::mat4 model = pose.mat4_cast() * glm::scale(scale);


	// debug vis
	//auto app(neb::gfx::app::__gfx_glsl::global().lock());

	//if(!flag_.all(neb::core::core::scene::util::flag::PHYSX_VISUALIZATION)) return;

	// visual debugging
	//if(px_scene_)
	{
		//const physx::PxRenderBuffer& rb = px_scene_->getRenderBuffer();

		//physx::PxU32 nblines = rb.getNbLines();
		//const physx::PxDebugLine* lines = rb.getLines();

		//physx::PxU32 nbtriangles = rb.getNbTriangles();
		//const physx::PxDebugTriangle* triangles = rb.getTriangles();


		physx::PxU32 nblines = nbVerts_;
		physx::PxU32 nbtriangles = 0;

		LOG(lg, neb::gfx::sl, debug) << "Debug visualization";
		//LOG(lg, neb::gfx::sl, debug) << "number of points    " << rb.getNbPoints();
		LOG(lg, neb::gfx::sl, debug) << "number of lines     " << nblines;
		LOG(lg, neb::gfx::sl, debug) << "number of triangles " << nbtriangles;

		// argb 32 bit
		physx::PxU32 red = 0xffff0000;

		std::vector<physx::PxDebugLine> lines;
		for(unsigned int i = 0; i < nbVerts_; i++)
		{
			if((i % 100) == 0) LOG(lg, neb::gfx::sl, debug) << std::dec << i;


			// calculate model space position and normal
			glm::vec3 p = vec3(model * vec4(vertices_[i].p,1));
			glm::vec3 n = mat3(model) * vertices_[i].n;
			glm::vec3 e = p + n;

			lines.emplace_back(
					physx::PxVec3(p.x, p.y, p.z),
					physx::PxVec3(e.x, e.y, e.z),
					red);
		}


		physx::PxDebugTriangle* triangles = NULL;


		//auto e = neb::could_be<neb::gfx::environ::base, neb::gfx::environ::three>(context->environ_);
		//if(e)
		{

			//glClear(GL_DEPTH_BUFFER_BIT);




			GLint i_color = p->attrib_table_[neb::gfx::glsl::attribs::COLOR];

			glEnableVertexAttribArray(p->attrib_table_[neb::gfx::glsl::attribs::POSITION]);
			if(i_color > -1)
				glEnableVertexAttribArray(i_color);

			GLuint buf;
			glGenBuffers(1, &buf);
			glBindBuffer(GL_ARRAY_BUFFER, buf);


			// lines
			glBufferData(
					GL_ARRAY_BUFFER,
					sizeof(physx::PxDebugLine) * nblines,
					&lines[0],
					GL_STREAM_DRAW
				    );

			glVertexAttribPointer(
					p->attrib_table_[neb::gfx::glsl::attribs::POSITION],
					3,
					GL_FLOAT,
					GL_FALSE,
					16,
					0);


			if(i_color > -1)
				glVertexAttribIPointer(
						p->attrib_table_[neb::gfx::glsl::attribs::COLOR],
						1,
						GL_UNSIGNED_INT,
						16,
						(GLvoid*)12);

			glDrawArrays(GL_LINES, 0, nblines * 2);

			checkerror("");

			// triangles
			glBufferData(
					GL_ARRAY_BUFFER,
					sizeof(physx::PxDebugTriangle) * nbtriangles,
					triangles,
					GL_STREAM_DRAW
				    );

			glVertexAttribPointer(
					p->attrib_table_[neb::gfx::glsl::attribs::POSITION],
					3,
					GL_FLOAT,
					GL_FALSE,
					16,
					0);


			if(i_color > -1)
				glVertexAttribIPointer(
						p->attrib_table_[neb::gfx::glsl::attribs::COLOR],
						1,
						GL_UNSIGNED_INT,
						16,
						(GLvoid*)12);

			glDrawArrays(GL_TRIANGLES, 0, nbtriangles * 3);

			checkerror("");

			// cleanup
			glDisableVertexAttribArray(p->attrib_table_[neb::gfx::glsl::attribs::POSITION]);
			if(i_color > -1)
				glDisableVertexAttribArray(p->attrib_table_[neb::gfx::glsl::attribs::COLOR]);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}

}


/*
   void			neb::gfx::mesh::tri1::draw_elements(
   program_shared						p,
   std::shared_ptr<neb::gfx::glsl::buffer::tri1>		buf,
   neb::core::pose const & pose,
   glm::vec3 scale)
   {
   LOG(lg, neb::gfx::sl, debug) << __PRETTY_FUNCTION__;

   buf->vertexAttribPointer();
// load modelview matrix
LOG(lg, neb::gfx::sl, debug) << "load modelview matrix";
mat4 model = pose.mat4_cast() * glm::scale(scale);

assert(p->uniform_table_[neb::gfx::glsl::uniforms::MODEL] != -1);

glUniformMatrix4fv(
p->uniform_table_[neb::gfx::glsl::uniforms::MODEL],
1,
GL_FALSE,
&model[0][0]
);

// bind
LOG(lg, neb::gfx::sl, debug) << "bind vbo";
buf->bind();

// draw
LOG(lg, neb::gfx::sl, debug) << "draw";
glDrawElements(
GL_TRIANGLES,
nbIndices_,
GL_UNSIGNED_SHORT,
0);

// unbind
LOG(lg, neb::gfx::sl, debug) << "unbind vbo";
buf->unbind();
}
*/
