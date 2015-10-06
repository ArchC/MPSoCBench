#ifdef POWER_SIM
#include <powersc.h>

/* Data struct definition. You should think that it is a row in a table. Each profile will have a certain number of tables. 
	 The basic idea is use a profile, with a pre-fixed number of operational frequencies. Each frequency, with a specific 
	 table of values */

// This group should be parameters, not defines
#define IS_START_PROFILE 0

#define IS_POWER_TABLE_FILE "../../is/tlm_noc_at/powersc/table_hermes_noc_50MHz.csv"

#define IS_WINDOW_REPORT
#define IS_WINDOW_REPORT_FILE "window_power_report"

#define IS_START_WINDOW_SIZE 10000

#define IS_MAX_LINESIZE_CSV_FILE 10240 
//#define MAX_INSTR_NAME_SIZE 30
#define IS_MAX_POWER_STATS_NAME_SIZE 30
#define IS_MAX_POWER_STATS_DESCR_SIZE 140

#define IS_TYPE_LINE_NUM_PROFILE 0
#define IS_TYPE_LINE_PROFILE     1
#define IS_TYPE_LINE_IDLE        2
#define IS_TYPE_LINE_ACTIVE      3

class is_power_stats {
	private:
		struct profile {
			char power_stats_name[IS_MAX_POWER_STATS_NAME_SIZE];
			char power_stats_descr[IS_MAX_POWER_STATS_DESCR_SIZE];
			unsigned int freq;
			double freq_scale;
			double power_scale;
      double idle_power;    //power when node is not transmiting data
      double active_power;  //power when node is transmiting data
		};

		struct power_stats_data {
			profile* p;
		};

		struct dynamic_data {
#ifdef IS_WINDOW_REPORT
			long long window_num_flits;
			double window_energy;
			double window_power;
			long long window_count;
			unsigned int window_size;
		  //double execution_time;
      double last_window_time;
#endif

			long long total_num_flits; 
			double total_energy;
			double total_power;

			unsigned int actual_profile;
			unsigned int num_profiles;
		};

		dynamic_data dyn;
		power_stats_data psc_data;

#ifdef IS_WINDOW_REPORT
		FILE* out_window_power_report;
#endif
    int total_nodes;

	public:
		psc_cell_power_info psc_info;

		// Constructor
  
  is_power_stats(const char* node_name, int n_nodes): psc_info(node_name, "NoC"), total_nodes(n_nodes) 
	{
	
      PSC_NUM_FIRST_SAMPLES(0x7FFFFFFF);
			init(IS_POWER_TABLE_FILE);
			dyn.actual_profile = IS_START_PROFILE;
			
			dyn.total_num_flits = 0;
			dyn.total_energy = 0;
			dyn.total_power = 0;
#ifdef IS_WINDOW_REPORT
			dyn.window_size = IS_START_WINDOW_SIZE;

			dyn.window_num_flits = 0;
			dyn.window_energy = 0;
			dyn.window_power = 0;
			dyn.window_count = 0;
			//dyn.execution_time = 0;
      dyn.last_window_time = 0;

			char filename[512];
			strcpy(filename, IS_WINDOW_REPORT_FILE);
			strcat(filename, "_");
			strcat(filename, node_name);
			strcat(filename, ".csv");
			out_window_power_report = fopen(filename, "w");
			if (out_window_power_report == NULL) {
				perror("Couldn't open specified out_window_power_report file");
				exit(1);
			}
#endif
			//print_psc_data();
		}

		// Destructor
		~is_power_stats() {
			//free(psc_data.p);

#ifdef IS_WINDOW_REPORT
			fclose(out_window_power_report);
#endif
		}

		double get_power() {
			return psc_info.get_power();
		}

		double get_energy_active(int flits, int profile) {
      // [J] = [W] / [1/s] = [W] * [s]
			return (flits * psc_data.p[profile].active_power * psc_data.p[profile].power_scale / 
        psc_data.p[profile].freq_scale * psc_data.p[profile].freq);
		}

    int type_line(int line, int num_profiles) {
      if (num_profiles == 0) {
        return IS_TYPE_LINE_NUM_PROFILE;
      } 
      else if (line <= num_profiles + 1) {
        return IS_TYPE_LINE_PROFILE;
      }
      else if (line == num_profiles + 2) {
       return IS_TYPE_LINE_IDLE;
      }
      else
        return IS_TYPE_LINE_ACTIVE;
    }

#ifdef IS_WINDOW_REPORT
		void incr_window_energy(double v) {
			dyn.window_energy += v;
		}

		void reset_window_data() {
			dyn.window_num_flits = 0;
			dyn.window_energy = 0;
			dyn.window_power = 0;
		}

		void calc_window_power() {
      double window_total_time = sc_time_stamp().to_seconds() - dyn.last_window_time;
      dyn.last_window_time = sc_time_stamp().to_seconds();
      double window_active_time = dyn.window_num_flits / psc_data.p[dyn.actual_profile].freq_scale * psc_data.p[dyn.actual_profile].freq;
      dyn.window_power = (dyn.window_energy + (total_nodes*window_total_time - window_active_time) * psc_data.p[dyn.actual_profile].idle_power )/ window_total_time; 
		}

		void window_power_report() {
			fprintf(out_window_power_report, "%d,%.10lf,%lld,%.10lf\n", dyn.actual_profile, dyn.last_window_time, dyn.window_count, dyn.window_power);
		}
#endif

		void incr_total_energy(double v) {
			dyn.total_energy += v;
		}

    //void incr_execution_time(int num_flits, int p) {
		//  dyn.execution_time += num_flits / (psc_data.p[dyn.actual_profile].freq * psc_data.p[dyn.actual_profile].freq_scale);
    //}

		void update_stat_power(int flits) {
  		dyn.total_num_flits+=flits;
			incr_total_energy(get_energy_active(flits, dyn.actual_profile));
      //incr_execution_time(flits, dyn.actual_profile);
#ifdef IS_WINDOW_REPORT
			dyn.window_num_flits+=flits;
			incr_window_energy(get_energy_active(flits, dyn.actual_profile));
			if (dyn.window_num_flits == dyn.window_size) {
				dyn.window_count++;
				calc_window_power();
				window_power_report();
				reset_window_data();
			}
#endif
		}

		void calc_total_power() {
      double total_time = sc_time_stamp().to_seconds();
      double total_active_time = dyn.total_num_flits / (psc_data.p[dyn.actual_profile].freq_scale * psc_data.p[dyn.actual_profile].freq);
      //printf("Total flits: %lld; Total time: %g; Total active time: %g;\n", dyn.total_num_flits, total_time, total_active_time);
      //##Total power = Sum[n_flits * P_active + (Ttotal-n_flits)*P_idle];
			dyn.total_power = ( dyn.total_energy + (total_nodes*total_time - total_active_time) * psc_data.p[dyn.actual_profile].idle_power )/ total_time;
		}

		void powersc_connect() {
			calc_total_power();
			psc_info.set_power(dyn.total_power);
			psc_info.set_level(PSC_RT_LEVEL);
			PSC_INSERT_CELL(psc_info);
		}

		void report() {
			PSC_REPORT_POWER;
		}

		char* next_strtok(const char* param, FILE* f, int pos_line) {
			char* pch = NULL;
			pch = strtok(NULL,param);
			if (pch == NULL) {
				printf("Error reading csv file, line %d. Unexpected format\n", pos_line);
				fclose(f);
				exit(1);
			}
			return pch;
		}

		// Read from file 
		void init(const char* filename) {
			FILE* f = NULL;
			char c = 0;
			char line[IS_MAX_LINESIZE_CSV_FILE];
			//char instr_name[MAX_INSTR_NAME_SIZE];
			char aux[IS_MAX_LINESIZE_CSV_FILE];
			double value;
      char* res;

			unsigned int index = 0;
			unsigned int pos_line = 0;
			unsigned int valid_line = 0;
      unsigned int profile_id = 0;
			fpos_t pos;

			char* pch = NULL;

			f = fopen(filename, "r");
			if (f == NULL) {
				printf("\ntentando abrir arquivoooooo csv");
				sprintf(aux, "Power file %s not found", filename);
				perror(aux);
				exit(1);
			}
      
      		dyn.num_profiles = 0; // Set a default value 

      		int state_id = 0;

			do {
				//fgetpos (f, &pos);
				res = fgets(line, IS_MAX_LINESIZE_CSV_FILE, f); // Get a new line from file
				if (feof(f)) break;
				pos_line++; // It says what line I am reading now
				pch = strtok(line, ",\""); // It parses the line

				if (pch[0] == '#'); // If it is a comment, ignore
				else if (pch == NULL) break;
				else { // Just found a valid new line
					valid_line++;
					// First Valid Line: number of profiles
          switch(type_line(valid_line, dyn.num_profiles)) {
            case IS_TYPE_LINE_NUM_PROFILE:
              dyn.num_profiles = atoi(pch);
					  	psc_data.p = (profile *)malloc(sizeof(profile) * dyn.num_profiles);
              for (int i = 0; i < dyn.num_profiles; i++) {
                psc_data.p[i].active_power = 0;
                psc_data.p[i].idle_power = 0;
              }
            break;
            case IS_TYPE_LINE_PROFILE:
					  	//profile_id = atoi(pch);
              profile_id = state_id++;

              if (profile_id >= dyn.num_profiles) {
                printf("Error: Invalid profile_id greater than num_profiles: %d > %d\n", 
                  profile_id, dyn.num_profiles);
              }
    					
              //pch = next_strtok(",\"", f, pos_line);
    					psc_data.p[profile_id].freq = atoi(pch);
    					
    					pch = next_strtok(",\"", f, pos_line);
    					psc_data.p[profile_id].freq_scale = atof(pch);
    					
    					pch = next_strtok(",\"", f, pos_line);
    					psc_data.p[profile_id].power_scale = atof(pch);
    					
    					pch = next_strtok(",\"", f, pos_line);
    					strcpy(psc_data.p[profile_id].power_stats_name, pch);
    					
    					pch = next_strtok(",\"", f, pos_line);
    					strcpy(psc_data.p[profile_id].power_stats_descr, pch);
            break;
            case IS_TYPE_LINE_IDLE:
    					psc_data.p[0].idle_power = atof(pch);

    					for(int i = 1; i < dyn.num_profiles;i++) {
    						pch = next_strtok(",\"", f, pos_line);
    						psc_data.p[i].idle_power = atof(pch);
    					}
            break;
            default: // IS_TYPE_LINE_ACTIVE
    					psc_data.p[0].active_power = atof(pch);

    					for(int i = 1; i < dyn.num_profiles;i++) {
    						pch = next_strtok(",\"", f, pos_line);
    						psc_data.p[i].active_power = atof(pch);
    					}
            break;
          }
				}
			} while(!feof(f));

			fclose(f);
		}

		void print_psc_data() {
			int i = 0, p = 0;

			for(p = 0; p < dyn.num_profiles; p++) {
				printf("Profile %d\n", p);
				printf("Name: %s\n", psc_data.p[p].power_stats_name);
				printf("Description: %s\n\n", psc_data.p[p].power_stats_descr);
			}

			printf("NoC State");
			for(p = 0; p < dyn.num_profiles; p++) {
				printf(" | Power Profile %d", p);
			}
			printf("\n");
      printf("Inactive  ");
      for(p = 0; p < dyn.num_profiles; p++) {
        printf(" | %15.3lf", psc_data.p[p].idle_power);
      }
      printf("\n");
      printf("Active    ");
      for(p = 0; p < dyn.num_profiles; p++) {
        printf(" | %15.3lf", psc_data.p[p].active_power);
      }
      printf("\n");
		}
};

#endif
