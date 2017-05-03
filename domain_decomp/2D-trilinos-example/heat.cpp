#include "BelosCGIter.hpp"
#include "BelosOutputManager.hpp"
#include "DomainSignatureCollection.h"
#include "FunctionWrapper.h"
#include "MyTypeDefs.h"
#include "OpATA.h"
#include "ZeroSum.h"
#include "args.h"
//#include <Amesos2.hpp>
//#include <Amesos2_Version.hpp>
#include <BelosBiCGStabSolMgr.hpp>
#include <BelosBlockCGSolMgr.hpp>
#include <BelosGCRODRSolMgr.hpp>
#include <BelosLSQRSolMgr.hpp>
#include <BelosBlockGmresSolMgr.hpp>
#include <BelosConfigDefs.hpp>
#include <BelosLinearProblem.hpp>
#include <BelosTpetraAdapter.hpp>
#include <MatrixMarket_Tpetra.hpp>
#include <Teuchos_Comm.hpp>
#include <Teuchos_GlobalMPISession.hpp>
#include <Teuchos_ParameterList.hpp>
#include <Teuchos_RCP.hpp>
#include <Teuchos_Tuple.hpp>
#include <Teuchos_VerboseObject.hpp>
#include <Teuchos_oblackholestream.hpp>
#include <BelosLSQRSolMgr.hpp>
#include <chrono>
#include <cmath>
#include <iostream>
#include <iostream>
//#include <mpi.h>
#include <string>
#include <unistd.h>

using Teuchos::RCP;
using Teuchos::rcp;

// =========== //
// main driver //
// =========== //

using namespace std;
int main(int argc, char *argv[])
{
	//    using Belos::FuncWrap;
	using Teuchos::RCP;
	using Teuchos::rcp;
	using namespace std::chrono;

	Teuchos::GlobalMPISession global(&argc, &argv, nullptr);
	RCP<const Teuchos::Comm<int>> comm = Tpetra::DefaultPlatform::getDefaultPlatform().getComm();

	int num_procs = comm->getSize();

	int my_global_rank = comm->getRank();

	// parse input
	args::ArgumentParser  parser("");
	args::HelpFlag        help(parser, "help", "Display this help menu", {'h', "help"});

	args::ValueFlag<int> f_n(parser, "n", "number of cells in the x direction, in each domain",
	                          {'n'});
	args::ValueFlag<string> f_mesh(parser, "file_name", "read in a mesh", {"mesh"});
	args::ValueFlag<int> f_square(
	parser, "num_domains", "create a num_domains x num_domains square of grids", {"square"});
	args::ValueFlag<int> f_amr(parser, "num_domains", "create a num_domains x num_domains square "
	                                                   "of grids, and a num_domains*2 x "
	                                                   "num_domains*2 refined square next to it",
	                            {"amr"});
	args::Flag           f_outclaw(parser, "outclaw", "output amrclaw ascii file", {"outclaw"});
	args::ValueFlag<int> f_l(parser, "n", "run the program n times and print out the average",
	                         {'l'});
	args::ValueFlag<string> f_m(parser, "matrix filename", "the file to write the matrix to",
	                            {'m'});
	args::ValueFlag<string> f_s(parser, "solution filename", "the file to write the solution to",
	                            {'s'});
	args::ValueFlag<string> f_resid(parser, "residual filename",
	                                "the file to write the residual to", {"residual"});
	args::ValueFlag<string> f_error(parser, "error filename",
	                                "the file to write the error to", {"error"});
	args::ValueFlag<string> f_r(parser, "rhs filename", "the file to write the rhs vector to",
	                            {'r'});
	args::ValueFlag<string> f_g(parser, "gamma filename", "the file to write the gamma vector to",
	                            {'g'});
	args::ValueFlag<string> f_read_gamma(parser, "gamma filename",
	                                     "the file to read gamma vector from", {"readgamma"});
	args::ValueFlag<string> f_flux(parser, "flux filename", "the file to write flux difference to",
	                            {"flux"});
	args::ValueFlag<string> f_p(parser, "preconditioner filename",
	                            "the file to write the preconditioner to", {'p'});
	args::ValueFlag<double> f_t(
	parser, "tolerance", "set the tolerance of the iterative solver (default is 1e-10)", {'t'});
    args::ValueFlag<int> f_d(
	parser, "row", "pin gamma value to zero (by modifying that row of the schur compliment matrix)",
	{'z'});
	args::Flag f_pinv(parser, "wrapper", "compute using pseudoinverse", {"pinv"});
	args::Flag f_wrapper(parser, "wrapper", "use a function wrapper", {"wrap"});
	args::Flag f_gauss(parser, "gauss", "solve gaussian function", {"gauss"});
	args::Flag f_oldgauss(parser, "gauss", "solve gaussian function", {"oldgauss"});
	args::Flag f_prec(parser, "prec", "use block diagonal preconditioner", {"prec"});
	args::Flag f_precata(parser, "prec", "use block diagonal preconditioner", {"precata"});
	args::Flag f_neumann(parser, "neumann", "use neumann boundary conditions", {"neumann"});
	args::Flag f_cg(parser, "gmres", "use CG for iterative solver", {"cg"});
	args::Flag f_gmres(parser, "gmres", "use GMRES for iterative solver", {"gmres"});
	args::Flag f_lsqr(parser, "gmres", "use GMRES for iterative solver", {"lsqr"});
	args::Flag f_rgmres(parser, "rgmres", "use GCRO-DR (Recycling GMRES) for iterative solver",
	                    {"rgmres"});
	args::Flag f_bicg(parser, "gmres", "use BiCGStab for iterative solver", {"bicg"});
	args::Flag f_nozero(parser, "nozero", "don't make the average of vector zero in CG solver",
	                    {"nozero"});
	args::Flag f_lu(parser, "lu", "use LU decomposition", {"lu"});
	args::Flag f_ilu(parser, "ilu", "use incomplete LU preconditioner", {"ilu"});
	args::ValueFlag<int> f_iter(parser, "iterative", "use iterative method", {"iterative"});

	if (argc < 5) {
		if (my_global_rank == 0) std::cout << parser;
		return 0;
	}
	try {
		parser.ParseCLI(argc, argv);
	} catch (args::Help) {
		if (my_global_rank == 0) std::cout << parser;
		return 0;
	} catch (args::ParseError e) {
		if (my_global_rank == 0) {
			std::cerr << e.what() << std::endl;
			std::cerr << parser;
		}
		return 1;
	} catch (args::ValidationError e) {
		if (my_global_rank == 0) {
			std::cerr << e.what() << std::endl;
			std::cerr << parser;
		}
		return 1;
	}
	DomainSignatureCollection dsc;
	if (f_mesh) {
		string d = args::get(f_mesh);
		dsc      = DomainSignatureCollection(d, comm->getRank());
	} else if (f_amr) {
		int d = args::get(f_amr);
		dsc   = DomainSignatureCollection(d, d, comm->getRank(), true);
	} else {
        int d = args::get(f_square);
		dsc = DomainSignatureCollection(d, d, comm->getRank());
	}
	// Set the number of discretization points in the x and y direction.
	int    nx            = args::get(f_n);
	int    ny            = args::get(f_n);
	int    total_cells   = dsc.num_global_domains*nx*ny;
	if (f_amr) {
		total_cells *= 5;
	}

	if (dsc.num_global_domains < num_procs) {
		std::cerr << "number of domains must be greater than or equal to the number of processes\n";
		return 1;
	}
	// partition domains
	if (num_procs > 1) {
		dsc.zoltanBalance();
	}

	scalar_type tol = 1e-10;
	if (f_t) {
		tol = args::get(f_t);
	}

	int loop_count = 1;
	if (f_l) {
		loop_count = args::get(f_l);
	}

    int del = -1;
	if (f_d) {
		del = args::get(f_d);
	}

	string save_matrix_file = "";
	if (f_m) {
		save_matrix_file = args::get(f_m);
	}

	string save_solution_file = "";
	if (f_s) {
		save_solution_file = args::get(f_s);
	}

	string save_residual_file = "";
	if (f_resid) {
		save_residual_file = args::get(f_resid);
	}
	string save_error_file = "";
	if (f_error) {
		save_error_file = args::get(f_error);
	}
	string save_rhs_file = "";
	if (f_r) {
		save_rhs_file = args::get(f_r);
	}
	string save_gamma_file = "";
	if (f_g) {
		save_gamma_file = args::get(f_g);
	}

	string save_prec_file = "";
	if (f_p) {
		save_prec_file = args::get(f_p);
	}

    
	// the functions that we are using
	vector<double> xval
	= {0.07768174089481361, 0.4208838472612919,  0.9473139111796449,  0.5089692183323905,
	   0.9570464247595821,  0.15905126169737582, 0.11849894156700524, 0.40999270280625555,
	   0.255893751363913,   0.4596839214805539,  0.48802004339584437, 0.13220719550284032,
	   0.15010376352950006, 0.6220357288995026,  0.5745530849579297,  0.19647252248155644,
	   0.22117387776294362, 0.20684448971820446, 0.24214199341522935, 0.7996623718773789};

	vector<double> yval
	= {0.2626206022801887,  0.5901019591033256, 0.22507728085248468, 0.6589237056448219,
	   0.16792176055765118, 0.7467770932376991, 0.586093747878745,   0.20084730007838192,
	   0.2305544870010452,  0.6032035084811954, 0.7616342948448442,  0.07395053135567398,
	   0.3145108689728848,  0.7994789501256063, 0.31702627032281594, 0.9410740272588111,
	   0.4335486185679469,  0.8896958913102787, 0.8237038487836723,  0.5312785837478274};

	vector<double> alpha = {
	57.12450253383478, 99.4615147296493,  55.26433103539979, 92.3500405527951,  87.67509863441407,
	76.12508998928006, 73.05691678209054, 84.58106997693446, 77.51740813583851, 89.11178685086824,
	79.61010770157222, 77.8956019831367,  76.6795489632914,  71.36011508633008, 99.5339946457969,
	59.75741874591391, 77.37272321552643, 91.75611892120241, 85.7168895671343,  68.55489734818805};

	function<double(double, double)> ffun;
	function<double(double, double)> gfun;
	function<double(double, double)> nfunx;
	function<double(double, double)> nfuny;

	if (f_oldgauss) {
		ffun = [xval, yval, alpha](double x, double y) {
			double retval = 0;
			for (int i = 0; i < 20; i++) {
				double xv = xval[i];
				double yv = yval[i];
				double a  = alpha[i];
				double r2 = (xv - x) * (xv - x) + (yv - y) * (yv - y);
				retval += 4 * a * (a * r2 - 1) * exp(-a * r2);
			}
			return retval;
		};

		gfun = [xval, yval, alpha](double x, double y) {
			double retval = 0;
			for (int i = 0; i < 20; i++) {
				double xv = xval[i];
				double yv = yval[i];
				double a  = alpha[i];
				double r2 = (xv - x) * (xv - x) + (yv - y) * (yv - y);
				retval += exp(-a * r2);
			}
			return retval;
		};

		nfunx = [xval, yval, alpha](double x, double y) {
			double retval = 0;
			for (int i = 0; i < 20; i++) {
				double xv = xval[i];
				double yv = yval[i];
				double a  = alpha[i];
				double r2 = (xv - x) * (xv - x) + (yv - y) * (yv - y);
				retval += 2 * a * (xv - x) * exp(-a * r2);
			}
			return retval;
		};

		nfuny = [xval, yval, alpha](double x, double y) {
			double retval = 0;
			for (int i = 0; i < 20; i++) {
				double xv = xval[i];
				double yv = yval[i];
				double a  = alpha[i];
				double r2 = (xv - x) * (xv - x) + (yv - y) * (yv - y);
				retval += 2 * a * (yv - y) * exp(-a * r2);
			}
			return retval;
		};
	} else if (f_gauss) {
		gfun = [](double x, double y) { return exp(cos(10 * M_PI * x)) - exp(cos(11 * M_PI * y)); };
		ffun = [](double x, double y) {
			return 100 * M_PI * M_PI * (pow(sin(10 * M_PI * x), 2) - cos(10 * M_PI * x))
			       * exp(cos(10 * M_PI * x))
			       + 121 * M_PI * M_PI * (cos(11 * M_PI * y) - pow(sin(11 * M_PI * y), 2))
			         * exp(cos(11 * M_PI * y));
		};
		nfunx = [](double x, double y) {
			return -10 * M_PI * sin(10 * M_PI * x) * exp(cos(10 * M_PI * x));
		};

		nfuny = [](double x, double y) {
			return 11 * M_PI * sin(11 * M_PI * y) * exp(cos(11 * M_PI * y));
		};
	} else {
		ffun = [](double x, double y) {
            x-=.3;
			return -5 * M_PI * M_PI * sin(M_PI * y) * cos(2 * M_PI * x);
		};
		gfun = [](double x, double y) {
            x-=.3;
			return sin(M_PI * y) * cos(2 * M_PI * x);
		};
		nfunx = [](double x, double y) {
            x-=.3;
			return -2 * M_PI * sin(M_PI * y) * sin(2 * M_PI * x);
		};
		nfuny = [](double x, double y) {
            x-=.3;
			return M_PI * cos(M_PI * y) * cos(2 * M_PI * x);
		};
	}

	valarray<double> times(loop_count);
	for (int loop = 0; loop < loop_count; loop++) {
		comm->barrier();
		
		steady_clock::time_point domain_start = steady_clock::now();

		DomainCollection dc(dsc, nx, comm);

		ZeroSum zs;
		if (f_neumann) {
			if (!f_nozero) {
				zs.setTrue();
			}
			dc.initNeumann(ffun, gfun, nfunx, nfuny, f_amr);
		} else {
			dc.initDirichlet(ffun, gfun);
            dc.amr = f_amr;
		}

		comm->barrier();
		steady_clock::time_point domain_stop = steady_clock::now();
		duration<double>         domain_time = domain_stop - domain_start;

		if (my_global_rank == 0)
			cout << "Domain Initialization Time: " << domain_time.count() << endl;

		// Create a map that will be used in the iterative solver
		RCP<map_type>       matrix_map       = dc.matrix_map;
		RCP<const map_type> matrix_map_const = dc.matrix_map;

		// Create the gamma and diff vectors
		RCP<vector_type> gamma = rcp(new vector_type(matrix_map, 1));
		RCP<vector_type> r     = rcp(new vector_type(matrix_map, 1));
		RCP<vector_type> x     = rcp(new vector_type(matrix_map, 1));
		RCP<vector_type> d     = rcp(new vector_type(matrix_map, 1));
		RCP<vector_type> diff  = rcp(new vector_type(matrix_map, 1));
		RCP<RBMatrix>    RBA;
		RCP<Tpetra::Operator<scalar_type>> op;

		// Create linear problem for the Belos solver
		RCP<Belos::LinearProblem<scalar_type, vector_type, Tpetra::Operator<scalar_type>>> problem;
		RCP<Belos::SolverManager<scalar_type, vector_type, Tpetra::Operator<scalar_type>>> solver;
		Teuchos::ParameterList belosList;
		if (dsc.num_global_domains != 1) {
			// do iterative solve

			// Get the b vector
			RCP<vector_type> b = rcp(new vector_type(matrix_map, 1));
			dc.solveWithInterface(*gamma, *b);

			if (save_rhs_file != "") {
				Tpetra::MatrixMarket::Writer<matrix_type>::writeDenseFile(save_rhs_file, b, "", "");
			}

			if (f_wrapper) {
				// Create a function wrapper
				op = rcp(new FuncWrap(b, &dc));
			} else { // if (f_rbmatrix || f_lu) {
				// Form the matrix
				comm->barrier();
				steady_clock::time_point form_start = steady_clock::now();

				RBA = dc.formRBMatrix(matrix_map, del);

				comm->barrier();
				duration<double> form_time = steady_clock::now() - form_start;

				if (my_global_rank == 0)
					cout << "Matrix Formation Time: " << form_time.count() << endl;

				if (save_matrix_file != "") {
					comm->barrier();
					steady_clock::time_point write_start = steady_clock::now();

					ofstream out_file(save_matrix_file);
					out_file << *RBA;
					out_file.close();

					comm->barrier();
					duration<double> write_time = steady_clock::now() - write_start;
					if (my_global_rank == 0)
						cout << "Time to write matrix to file: " << write_time.count() << endl;
				}
				if (f_pinv) {
					op = rcp(new OpATA(RBA));
				} else {
					op = RBA;
				}
			}

            if(f_pinv){
				RCP<vector_type> ATb = rcp(new vector_type(matrix_map, 1));
				RBA->apply(*b, *ATb);
				problem = rcp(
				new Belos::LinearProblem<scalar_type, vector_type, Tpetra::Operator<scalar_type>>(op, gamma, ATb));
			}else{
				problem = rcp(
				new Belos::LinearProblem<scalar_type, vector_type, Tpetra::Operator<scalar_type>>(op, gamma, b));
			}

			if (f_ilu) {
				comm->barrier();
				steady_clock::time_point prec_start = steady_clock::now();

				RCP<RBMatrix> L, U;
				RBMatrix      Copy = *RBA;
				Copy.ilu(L, U);
				RCP<LUSolver> solver = rcp(new LUSolver(L, U));
				problem->setLeftPrec(solver);

				comm->barrier();
				duration<double> prec_time = steady_clock::now() - prec_start;

				if (my_global_rank == 0)
					cout << "Preconditioner Formation Time: " << prec_time.count() << endl;

				// ofstream out_file("L.mm");
				// out_file << *L;
				// out_file.close();
				// out_file = ofstream("U.mm");
				// out_file << *U;
				// out_file.close();
			} else if (f_prec) {
				comm->barrier();
				steady_clock::time_point prec_start = steady_clock::now();

				RCP<RBMatrix> P = RBA->invBlockDiag();
				problem->setRightPrec(P);

				comm->barrier();
				duration<double> prec_time = steady_clock::now() - prec_start;

				if (my_global_rank == 0)
					cout << "Preconditioner Formation Time: " << prec_time.count() << endl;

				if (save_prec_file != "") {
					comm->barrier();
					steady_clock::time_point write_start = steady_clock::now();

					ofstream out_file(save_prec_file);
					out_file << *P;
					out_file.close();

					comm->barrier();
					duration<double> write_time = steady_clock::now() - write_start;
					if (my_global_rank == 0)
						cout << "Time to write preconditioner to file: " << write_time.count()
						     << endl;
				}
			} else if (f_precata) {
				comm->barrier();
				steady_clock::time_point prec_start = steady_clock::now();

				RCP<RBMatrix> P = RBA->invBlockDiagATA();
				problem->setRightPrec(P);

				comm->barrier();
				duration<double> prec_time = steady_clock::now() - prec_start;

				if (my_global_rank == 0)
					cout << "Preconditioner Formation Time: " << prec_time.count() << endl;

				if (save_prec_file != "") {
					comm->barrier();
					steady_clock::time_point write_start = steady_clock::now();

					ofstream out_file(save_prec_file);
					out_file << *P;
					out_file.close();

					comm->barrier();
					duration<double> write_time = steady_clock::now() - write_start;
					if (my_global_rank == 0)
						cout << "Time to write preconditioner to file: " << write_time.count()
						     << endl;
				}

			}

			if (f_read_gamma) {
				gamma = Tpetra::MatrixMarket::Reader<matrix_type>::readDenseFile(
				args::get(f_read_gamma), comm, matrix_map_const);
			} else if (f_lu) {
				comm->barrier();
				steady_clock::time_point lu_start = steady_clock::now();

				RCP<RBMatrix> L, U;
				RBA->lu(L,U);
				LUSolver solver(L,U);
				solver.apply(*b, *gamma);

				comm->barrier();
				duration<double> lu_time = steady_clock::now() - lu_start;
				if (my_global_rank == 0) std::cout << "LU Time: " << lu_time.count() << endl;

				// ofstream out_file("L.mm");
				// out_file << *L;
				// out_file.close();
				// out_file = ofstream("U.mm");
				// out_file << *U;
				// out_file.close();

			} else {
				problem->setProblem();

				comm->barrier();
				steady_clock::time_point iter_start = steady_clock::now();
				// Set the parameters
				belosList.set("Block Size", 1);
				belosList.set("Maximum Iterations", 5000);
				belosList.set("Convergence Tolerance", tol);
				int verbosity = Belos::Errors + Belos::StatusTestDetails + Belos::Warnings
				                + Belos::TimingDetails + Belos::Debug;
				belosList.set("Verbosity", verbosity);
				// belosList.set("Orthogonalization", "ICGS");
                //
                belosList.set("Rel RHS Err",0.0);
                belosList.set("Rel Mat Err",0.0);

				// Create solver and solve
				if (f_lsqr) {
					solver = rcp(new Belos::LSQRSolMgr<scalar_type, vector_type,
					                                     Tpetra::Operator<scalar_type>>(
					problem, rcp(&belosList, false)));
				} else if (f_rgmres) {
					solver = rcp(new Belos::GCRODRSolMgr<scalar_type, vector_type,
					                                     Tpetra::Operator<scalar_type>>(
					problem, rcp(&belosList, false)));
				} else if (f_cg) {
					solver = rcp(new Belos::BlockCGSolMgr<scalar_type, vector_type,
					                                      Tpetra::Operator<scalar_type>>(
					problem, rcp(&belosList, false)));
				} else if (f_bicg) {
					solver = rcp(new Belos::BiCGStabSolMgr<scalar_type, vector_type,
					                                       Tpetra::Operator<scalar_type>>(
					problem, rcp(&belosList, false)));
				} else {
					solver = rcp(new Belos::BlockGmresSolMgr<scalar_type, vector_type,
					                                         Tpetra::Operator<scalar_type>>(
					problem, rcp(&belosList, false)));
				}
				solver->solve();

				comm->barrier();
				duration<double> iter_time = steady_clock::now() - iter_start;
				if (my_global_rank == 0)
					std::cout << "Gamma Solve Time: " << iter_time.count() << endl;
			}
		}

		// Do one last solve
		comm->barrier();
		steady_clock::time_point solve_start = steady_clock::now();

		dc.solveWithInterface(*gamma);

		comm->barrier();
		duration<double> solve_time = steady_clock::now() - solve_start;

		if (my_global_rank == 0)
			std::cout << "Time to solve with given set of gammas: " << solve_time.count() << endl;

		dc.residual();
		double ausum2 = dc.integrateAU();
		double fsum2  = dc.integrateF();
		std::cout << u8"Σf-Au: " << fsum2 - ausum2 << endl;
		std::cout << u8"Σf: " << fsum2 << endl;
		std::cout << u8"ΣAu: " << ausum2 << endl;
		if (f_iter) {
			for (int i = 0; i < args::get(f_iter); i++) {
				dc.residual();
				dc.swapResidSol();
				x->putScalar(0);
				dc.solveWithInterface(*x, *r);
        
				solver->reset(Belos::ResetType::Problem);
				Tpetra::MatrixMarket::Writer<matrix_type>::writeDenseFile("rrr.mm", r, "", "");
				if (f_wrapper) {
					((FuncWrap *) op.getRawPtr())->setB(r);
				}
				if (f_pinv) {
					RCP<vector_type> ATr = rcp(new vector_type(matrix_map, 1));
					RBA->apply(*r, *ATr);
					problem->setProblem(x, ATr);
					problem->setProblem(x, r);
				}else{
					problem->setProblem(x, r);
				}
				solver->setProblem(problem);
				solver->solve();
				dc.solveWithInterface(*x, *d);
				dc.sumResidIntoSol();
			}
		}

		// Calcuate error
		RCP<map_type>    err_map = rcp(new map_type(-1, 1, 0, comm));
		Tpetra::Vector<> exact_norm(err_map);
		Tpetra::Vector<> diff_norm(err_map);

		if (f_neumann) {
			double usum = dc.uSum();
			double uavg;
			Teuchos::reduceAll<int, double>(*comm, Teuchos::REDUCE_SUM, 1, &usum, &uavg);
			uavg /= dc.getGlobalNumCells();
			double esum = dc.exactSum();
			double eavg;
			Teuchos::reduceAll<int, double>(*comm, Teuchos::REDUCE_SUM, 1, &esum, &eavg);
			eavg /= dc.getGlobalNumCells();

			if (my_global_rank == 0) {
				cout << "Average of computed solution: " << uavg << endl;
				cout << "Average of exact solution: " << eavg << endl;
			}

			exact_norm.getDataNonConst()[0] = dc.exactNorm(eavg);
			diff_norm.getDataNonConst()[0]  = dc.diffNorm(uavg, eavg);
		} else {
			exact_norm.getDataNonConst()[0] = dc.exactNorm();
			diff_norm.getDataNonConst()[0]  = dc.diffNorm();
		}
		double global_diff_norm;
		double global_exact_norm;
		global_diff_norm  = diff_norm.norm2();
		global_exact_norm = exact_norm.norm2();

		comm->barrier();
		steady_clock::time_point total_stop = steady_clock::now();
		duration<double>         total_time = total_stop - domain_start;

		double residual = dc.residual();
		double fnorm    = dc.fNorm();
		double ausum    = dc.integrateAU();
		double fsum     = dc.integrateF();
		if (my_global_rank == 0) {
			std::cout << "Total run time: " << total_time.count() << endl;
			std::cout << std::scientific;
			std::cout.precision(13);
			std::cout << "Error: " << global_diff_norm / global_exact_norm << endl;
			std::cout << "Residual: " << residual / fnorm << endl;
			std::cout << u8"ΣAu-Σf: " << ausum-fsum << endl;
			cout << std::fixed;
			cout.precision(2);
		}
		times[loop] = total_time.count();
		if (save_solution_file != "") {
			ofstream out_file(save_solution_file);
			dc.outputSolution(out_file);
			out_file.close();
			if (f_amr) {
				ofstream out_file(save_solution_file + ".amr");
				dc.outputSolutionRefined(out_file);
				out_file.close();
			}
		}
		if (save_residual_file != "") {
			ofstream out_file(save_residual_file);
			dc.outputResidual(out_file);
			out_file.close();
			if (f_amr) {
				ofstream out_file(save_residual_file + ".amr");
				dc.outputResidualRefined(out_file);
				out_file.close();
			}
		}
		if (save_error_file != "") {
			ofstream out_file(save_error_file);
			dc.outputError(out_file);
			out_file.close();
			if (f_amr) {
				ofstream out_file(save_error_file + ".amr");
				dc.outputErrorRefined(out_file);
				out_file.close();
			}
		}
		if (save_gamma_file != "") {
			Tpetra::MatrixMarket::Writer<matrix_type>::writeDenseFile(save_gamma_file, gamma, "",
			                                                          "");
		}
        if (f_flux){
            dc.getFluxDiff(*gamma);
			Tpetra::MatrixMarket::Writer<matrix_type>::writeDenseFile(args::get(f_flux), gamma, "",
			                                                          "");
		}
		if (f_outclaw) {
			dc.outputClaw();
		}
	}

	if (loop_count > 1 && my_global_rank == 0) {
		cout << std::fixed;
		cout.precision(2);
		std::cout << "Times: ";
		for (double t : times) {
			cout << t << " ";
		}
		cout << endl;
		cout << "Average: " << times.sum() / times.size() << endl;
	}

	return 0;
}
