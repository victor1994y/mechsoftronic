#include "cute.h"
#include "ide_listener.h"
#include "xml_listener.h"
#include "cute_runner.h"

#include <list>
#include "types.hpp"
#include "cncmachine.hpp"

using namespace std;
void DisplayMovement(CncMachine::TRAVERSALXY movement)
{
	cout<<"########################################"<<endl;
	cout<<"X_Position= "<<movement.X.Position<<endl;
	cout<<"X_StepDir = "<<movement.X.StepDir <<endl;
	cout<<"X_StepNum = "<<movement.X.StepNum <<endl;
	cout<<endl;
	cout<<"Y_Position= "<<movement.Y.Position<<endl;
	cout<<"Y_StepDir = "<<movement.Y.StepDir <<endl;
	cout<<"Y_StepNum = "<<movement.Y.StepNum <<endl;
	cout<<"########################################"<<endl;
}

typedef struct
{
	alt_u32 x;
	alt_u32 y;
} COORDINATE;


COORDINATE xy(alt_u32 x, alt_u32 y)
{
	COORDINATE pos;
	pos.x = x;
	pos.y = y;
	return pos;
}

/////////////////////////////////////////////////////////////////////////////
///@brief verify cncmachine sets the correct direction
///@details creates a list of coordinates then send it into cncmachine object
///			compare old coordinate with new coordinate to verify direction
/////////////////////////////////////////////////////////////////////////////
void InputCoordinateTest() {
	list<COORDINATE> triangle;
	triangle.push_back(xy(294366,84801));
	triangle.push_back(xy(175520,307195));
	triangle.push_back(xy(64840,380000));
	triangle.push_back(xy(0,0));

	list<COORDINATE>::iterator it;
	CncMachine raptor;
	COORDINATE pos;
	COORDINATE old;
	CncMachine::TRAVERSALXY data;
	CncMachine::Direction dir;
//	alt_32 val;
	old.x = 0;
	old.y = 0;
	for(it = triangle.begin(); it != triangle.end(); it++)
	{
		pos = *it;
		raptor.SetNextPosition(pos.x,pos.y);
		data = raptor.GetXYMovement();
		DisplayMovement(data);
		//compare with previous position to new direction is
		//set correctly with respect to old position
		//verify X direction
//		val = (alt_32)data.X.Position - (alt_32)old.x;
//		cout << "value position change = "<<val<<endl;
		dir = ((alt_32)data.X.Position - (alt_32)old.x)>0?
				CncMachine::up: CncMachine::down;
		ASSERT_EQUAL(dir,data.X.StepDir);
		//verify Y direction
		dir = ((alt_32)data.Y.Position - (alt_32)old.y)>0?
				CncMachine::up: CncMachine::down;
		ASSERT_EQUAL(dir,data.Y.StepDir);

		old.x = data.X.Position;
		old.y = data.Y.Position;

	}
}

void outOfRangeTest()
{
	//ASSERTM("start writing tests", false);
	CncMachine machine;
	//send triangle coordinates , triangle.nc
	machine.SetNextPosition(380001,84801);
	ASSERT_EQUAL(1,(int)machine.SetNextPosition(380001,84801));
	DisplayMovement(machine.GetXYMovement());


}

void thisIsATest() {
	//ASSERTM("start writing tests", false);
	CncMachine machine;
	//send triangle coordinates , triangle.nc
	machine.SetNextPosition(294366,84801);
	DisplayMovement(machine.GetXYMovement());
	machine.SetNextPosition(175520,307195);
	DisplayMovement(machine.GetXYMovement());
	machine.SetNextPosition(64840,84800);
	DisplayMovement(machine.GetXYMovement());

}

void runAllTests(int argc, char const *argv[]){
	cute::suite s;
	//TODO add your test here
	s.push_back(CUTE(outOfRangeTest));
	s.push_back(CUTE(InputCoordinateTest));
	s.push_back(CUTE(thisIsATest));

	cute::xml_file_opener xmlfile(argc,argv);
	cute::xml_listener<cute::ide_listener<> >  lis(xmlfile.out);
	cute::makeRunner(lis,argc,argv)(s, "AllTests");
}

int main(int argc, char const *argv[]){
    runAllTests(argc,argv);
    return 0;
}


