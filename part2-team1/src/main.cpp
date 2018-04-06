#include <cstdio>
#include <gui/MainWindow.h>

#include <QtWidgets>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	QCoreApplication::setOrganizationName("CppPraktikum");
	QCoreApplication::setApplicationName("BasiCT");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);

	MainWindow mainWidget;
	mainWidget.adjustSize(); //https://forum.qt.io/topic/21035/how-to-move-the-window-to-the-center-of-the-screen/3
	mainWidget.move(QApplication::desktop()->screen()->rect().center() - mainWidget.rect().center());
	mainWidget.show();

	//command line
	QCommandLineParser parser;
	parser.addPositionalArgument("input", "input volume");
	parser.addHelpOption();
	parser.process(app);
	const QStringList args = parser.positionalArguments();
	if (!args.empty())
	{
		//input file specified as command line parameter
		QString file = args[0];
		mainWidget.load(file);
	}

	return app.exec();
}
// \
//                       int
//                      _,l;\
//                     char*I,
//                    *O[]={"",
//                   "gjstu","t"
//                  "fdpoe","uij"
//                 "se","gpvsui",\
//                   "gjgui","t"
//                  "jyui","tfwf"
//                 "oui","fjhiui",
//                "ojoui","ufoui",\
//               "fmfwfoui","uxfmgu"
//              "i","b!qbsusjehf!jo!"
//             "b!qfbs!usff/\xb\xb",""
//                "uxp!uvsumf!epwf"
//               "t-\xb","uisff!gsf"
//              "odi!ifot-!","gpvs!d"
//             "bmmjoh!cjset-!","gjwf"
//            "!hpme!sjoht<\xb","tjy!h"
//           "fftf!b.mbzjoh-!","tfwfo!t"
//          "xbot!b.txjnnjoh-\xb","fjhiu"
//         "!nbjet!b.njmljoh-!","ojof!mbe"
//             "jft!ebodjoh-!","ufo!m"
//            "pset!b.mfbqjoh-\xb","fm"
//           "fwfo!qjqfst!qjqjoh-!","ux"
//          "fmwf!esvnnfst!esvnnjoh-!",""
//         "Po!uif!","!ebz!pg!Disjtunbt!n"
//        "z!usvf!mpwf!hbwf!up!nf\xb","boe"
//       "!"};int putchar(int);int main(void
//      ){while(l<(sizeof O/sizeof*O-2)/2-1){
//           I=O[_=!_?sizeof O/sizeof*O-
//          3:_<(sizeof(O)/sizeof*O-2)/2?
//         sizeof O/sizeof*O-2:_==(sizeof(
//        O)/sizeof*O-2)/2?++l,0:_<(sizeof(
//       O)/sizeof(*O))-3?(_-1)==(sizeof(O)/
//      sizeof*O-2)/2?sizeof O/sizeof*O-1:_-1
//     :_<sizeof(O)/sizeof*O-2?l+1:_<sizeof(O)
//    /sizeof*O-1?l+(sizeof O/sizeof(*O)-2)/2:(
//   sizeof(O)/sizeof*O-2)/2];while(*I){putchar(
//                   *I++-1);}}
//                   return 0;}

// //from http://forum.codecall.net/topic/43097-obfuscated-c-contest/?p=388582
