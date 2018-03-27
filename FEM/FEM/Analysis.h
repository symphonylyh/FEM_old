/**
 * @file Analysis.h
 * Abstract Analysis class for various types of analysis.
 *
 * @author Haohang Huang
 * @date Feburary 26, 2018
 * @note No efficiency optimization required on March 26, 2018.
 * @TODO Query point method.
 */

#ifndef Analysis_h
#define Analysis_h

#include "Mesh.h"

/* Abstract base Analysis class with shared public methods and pure virtual methods.
 *
 * Key concepts of during the design of this class:
 *
 * 1.
 *
 * 2.
 * Since all the information such as mesh, global stiffness, force, etc
 have been made public in Analysis class, by inheritance we have all those info
 in this derived class. To use them, just say mesh, globalStiffness is okay
 because we don't define the member variable of the same name.
 In addition, from a memory allocation view, the base class's memory space is allocated
 inside the memory space of this derived class. But these space are just copies
 from the base class, i.e., if we have a second derived class, these space are
 independent and uniquely of their own.
 */
class Analysis
{
    public:
        /**
         * Default constructor for Analysis.
         */
        Analysis();

        /**
         * Custom constructor to create the mesh from input file for analysis.
         *
         * @param fileName The path to the input file.
         */
        Analysis(std::string const & fileName);

        /**
         * Virtual destructor for abstract class.
         */
        virtual ~Analysis();

        /**
         * Assemble the global stiffness matrix from the local stiffness matrix
         * of each element.
         */
        void assembleStiffness();

        /**
         * Apply the load at nodes.
         */
        void applyForce();

        /**
         * Designate boundary condition at nodes.
         */
        void boundaryCondition();

        /**
         * Solve for nodal displacement using different approaches.
         */
        virtual void solveDisp() = 0;

        /**
         * Compute nodal strain and stress from the extrapolation of results at
         * Gaussian integration points and average the value over the mesh.
         */
        void computeStrainAndStress();

        // @TODO Query queryPoint(const double & x, const double & y) const; // create a Query object of disp, strain, stress and return it. Query needs interpolation based on shape function, and need to find the nearest neighbor or locate the element it's in. Should also check if post-processing software does this for us

        /**
         * Print the nodal displacement.
         */
        void printDisp() const;

        /**
         * Print the nodal force.
         */
        void printForce() const;

        /**
         * Print the nodal strain.
         */
        void printStrain() const;

        /**
         * Print the nodal stress.
         */
        void printStress() const;

    protected: // "protected" is a good choice. Only visible to the derived class

        /** The mesh information of the problem */
        Mesh mesh;

        /** The global stiffness matrix as a 2n-by-2n sparse matrix */
        SparseMatrix<double> globalStiffness;

        /** The nodal displacement 2n-by-1 vector */
        VectorXd nodalDisp;

        /** The nodal force 2n-by-1 vector */
        VectorXd nodalForce;

        /** The nodal strain n-by-4 matrix */
        MatrixXd nodalStrain;

        /** The nodal strain n-by-4 matrix */
        MatrixXd nodalStress;
};

#endif /* Analysis_h */
