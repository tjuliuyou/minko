#include "HalfEdge.hpp"

using namespace minko::parser::mk;

HalfEdge::HalfEdge(unsigned int startNodeId,
					unsigned int endNodeId,
					unsigned int edgeId)
{
	_startNodeId	= startNodeId;
	_endNodeId		= endNodeId;
	_edgeId			= edgeId;
}

bool
HalfEdge::indiceInEdge(unsigned int indice)
{
	return indice == _startNodeId || indice == _endNodeId;
}

bool
HalfEdge::indiceInFace(unsigned int indice)
{
	return indice == _face[0]->startNodeId() || indice == _face[1]->startNodeId() || indice == _face[2]->startNodeId();
}

unsigned int
HalfEdge::getThirdVertex()
{
	for (int i = 0; i < 3; i++)
	{
		if (_face[i]->startNodeId() != _startNodeId && _face[i]->startNodeId() != _endNodeId)
			return _face[i]->startNodeId();
	}
	
	return 0;
}

void 
HalfEdge::setFace(HalfEdgePtr he1,
				  HalfEdgePtr he2,
				  HalfEdgePtr he3)
{
	_face.push_back(he1);
	_face.push_back(he2);
	_face.push_back(he3);
}

namespace minko
{
	namespace parser
	{
		namespace mk
		{
			std::ostream &operator<<(std::ostream &flux, const HalfEdge& halfEdge)
			{
				
				flux << "HalfEdge " << halfEdge.edgeId() << " : " << halfEdge.startNodeId() << " - " << halfEdge.endNodeId() << std::endl;
				flux << "  triangle : " << halfEdge.face()[0]->startNodeId() << ", " << halfEdge.face()[1]->startNodeId() << ", " << halfEdge.face()[2]->startNodeId() << std::endl;
				flux <<	"  next     : " << halfEdge.next()->startNodeId() << " - " << halfEdge.next()->endNodeId() << std::endl;
				flux <<	"  prec     : " << halfEdge.prec()->startNodeId() << " - " << halfEdge.prec()->endNodeId() << std::endl;
				if (halfEdge.adjacent() != NULL)
					flux << "  adjacent : " << halfEdge.adjacent()->startNodeId() << " - " << halfEdge.adjacent()->endNodeId() << std::endl;
				
				return flux;
			}

			std::ostream &operator<<(std::ostream &flux, std::shared_ptr<HalfEdge> halfEdge)
			{
				const HalfEdge& he = *halfEdge;

				return flux << he;
			}
		}
	}
}