#pragma once

namespace Tmpl8
{

class QuadTree{
	public:
		QuadTree(int depth);
		void insert();
		void query();
	private:
		void split();
		bool hasChildren;
		int x, y, w, h;
		QuadTree* Children;
};

}
