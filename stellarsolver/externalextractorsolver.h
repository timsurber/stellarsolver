/*  ExternalExtractorSolver, StellarSolver Intenal Library developed by Robert Lancaster, 2020

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
*/

#pragma once

#include "internalextractorsolver.h"
#include <QProcess>
#include <QPointer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

using namespace SSolver;

class ExternalExtractorSolver : public InternalExtractorSolver
{
        Q_OBJECT
    public:
        explicit ExternalExtractorSolver(ProcessType type, ExtractorType sexType, SolverType solType,
                                          FITSImage::Statistic imagestats, uint8_t const *imageBuffer, QObject *parent = nullptr);
        ~ExternalExtractorSolver();


        int extract() override;
        void abort() override;
        ExtractorSolver * spawnChildSolver(int n) override;

        QStringList getSolverArgsList();
        bool generateAstrometryConfigFile();

        QString fileToProcess;
        bool fileToProcessIsTempFile = false;
        int selectedStar;

        //External Options
        bool cleanupTemporaryFiles = true;
        bool autoGenerateAstroConfig = true;
        bool onlySendFITSFiles = true;

        //System File Paths
        QString confPath;               //Path to the Astrometry Config File
        QString sextractorBinaryPath;   //Path to the SExtractor Program binary
        QString solverPath;             //Path to the Astrometry Solver binary
        QString astapBinaryPath;        //Path to the ASTAP Program binary
        QString watneyBinaryPath;       //Path to the Watney Astrometry Solver Program binary
        QString wcsPath;                //Path to the WCSInfo binary

        //Methods to get default file paths
        static ExternalProgramPaths getLinuxDefaultPaths();
        static ExternalProgramPaths getLinuxInternalPaths();
        static ExternalProgramPaths getMacHomebrewPaths();
        static ExternalProgramPaths getWinANSVRPaths();
        static ExternalProgramPaths getWinCygwinPaths();

        //Method to set file paths
        void setExternalFilePaths(ExternalProgramPaths paths);

        // This is the xyls file path that we will be saving for Astrometry.net
        // If it is not set, it will be set to a random temporary file
        QString starXYLSFilePath;
        bool starXYLSFilePathIsTempFile = false; //This will be set to true if it gets generated

        QString solutionFile;

        void logSolver();
        void logSextractor();

        int writeStarExtractorTable();
        int getStarsFromXYLSFile();
        bool getSolutionInformation();
        bool getASTAPSolutionInformation();
        bool getWatneySolutionInformation();
        int saveAsFITS();
        void cleanupTempFiles() override;

        int loadWCS();
        bool appendStarsRAandDEC(QList<FITSImage::Star> &stars) override;

        bool pixelToWCS(const QPointF &pixelPoint, FITSImage::wcs_point &skyPoint) override;
        bool wcsToPixel(const FITSImage::wcs_point &skyPoint, QPointF &pixelPoint) override;


        /// WCS Struct
        struct wcsprm *m_wcs
        {
            nullptr
        };
        int m_nwcs = 0;

        //These are used for reading and writing the star extractor file
        char* xcol = strdup("X_IMAGE"); //This is the column for the x-coordinates
        char* ycol = strdup("Y_IMAGE"); //This is the column for the y-coordinates
        char* magcol = strdup("MAG_AUTO"); //This is the column for the magnitude
        char* colFormat = strdup("1E"); //This Format means a decimal number
        char* colUnits = strdup("pixels"); //This is the unit for the xy columns in the file
        char* magUnits = strdup("magnitude"); //This is the unit for the magnitude in the file

        int runExternalExtractor();

    private:
        QPointer<QProcess> solver;
        QPointer<QProcess> sextractorProcess;

        void run()
        override; //This starts the ExternalExtractorSolver in a separate thread.  Note, ExternalExtractorSolver uses QProcess


        int runExternalSolver();
        int runExternalASTAPSolver();
        int runExternalWatneySolver();

};
