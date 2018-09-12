/* -*- c-basic-offset: 8 -*-
 * Copyright (c) 2018 Open Grid Computing, Inc. All rights reserved.
 * Copyright (c) 2017 Sandia Corporation. All rights reserved.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the U.S. Government.
 * Export of this program may require a license from the United States
 * Government.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the BSD-type
 * license below:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *      Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *      Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 *      Neither the name of Sandia nor the names of any contributors may
 *      be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 *      Neither the name of Open Grid Computing nor the names of any
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *      Modified source versions must be plainly marked as such, and
 *      must not be misrepresented as being the original software.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <strings.h>
#include <string.h>
#include <pwd.h>
#include <time.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <sys/queue.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <slurm/spank.h>
#include <coll/rbt.h>
#include "jobinfo.h"

/*
 * This is a SLURM SPANK plugin that writes job data to a file when
 * the job is created, and again when the job completed.
 */

SPANK_PLUGIN(jobinfo, 2)

typedef struct job_pid {
	pid_t j_pid;
	LIST_ENTRY(job_pid) entry;
} *job_pid_t;

struct rbt job_tree;

typedef struct job_info_s {
	u_int		j_id;		/* S_JOB_ID */
	u_int		j_app_id;	/* S_TASK_ID */
	/* Collection of S_TASK_PID */
	LIST_HEAD(job_list_s, job_pid) j_pid_list;
	u_int		j_nnodes;	/* S_JOB_NNODES */
	u_int		j_ncpus;	/* S_JOB_NCPUS */
	u_int		j_step_id;	/* S_JOB_STEPID */
	u_int		j_init_count;	/* Tasks initialized */
	u_int		j_local_task_count; /* S_JOB_LOCAL_TASK_COUNT */

	long int	j_start;	/* time() */
	long int	j_end;		/* time() */

	u_int		j_status;	/* JOB_STARTED, JOB_EXITED */
	u_int		j_exit_status;	/* S_TASK_EXIT_STATUS */

	uid_t		j_user_id;	/* S_JOB_UID */
	char		*j_name;	/* SLURM_JOB_NAME */

	struct rbn	j_rbn;
} *job_info_t;

job_info_t find_job(u_int job_id)
{
	job_info_t job;
	struct rbn *rbn = rbt_find(&job_tree, &job_id);
	if (!rbn)
		return NULL;
	return container_of(rbn, struct job_info_s, j_rbn);
}

void free_job(job_info_t job)
{
	job_pid_t jp;

	rbt_del(&job_tree, &job->j_rbn);

	while (!LIST_EMPTY(&job->j_pid_list)) {
		jp = LIST_FIRST(&job->j_pid_list);
		LIST_REMOVE(jp, entry);
		free(jp);
	}
	if (job->j_name)
		free(job->j_name);
	free(job);
}

int update_job_info(job_info_t job)
{
	int rc = 0;
	FILE *f;
	char *datafile;
	char path[PATH_MAX];
	struct passwd *pw;
	job_pid_t jp;

	datafile = getenv("LDMS_JOBINFO_DATA_FILE");
	if (datafile == NULL)
		datafile = LDMS_JOBINFO_DATA_FILE;

	f = fopen(datafile, "w");
	if (f == NULL)
		return errno;

	rc = fprintf(f, "JOB_ID=%d\n", job->j_id);
	rc = fprintf(f, "JOB_STEP_ID=%d\n", job->j_step_id);
	rc = fprintf(f, "JOB_STATUS=%d\n", job->j_status);
	rc = fprintf(f, "JOB_APP_ID=%d\n", job->j_app_id);
	rc = fprintf(f, "JOB_USER_ID=%d\n", job->j_user_id);
	rc = fprintf(f, "JOB_START=%ld\n", job->j_start);
	rc = fprintf(f, "JOB_END=%ld\n", job->j_end);
	rc = fprintf(f, "JOB_EXIT=%d\n", job->j_exit_status);
	rc = fprintf(f, "JOB_NNODES=%ld\n", job->j_nnodes);
	rc = fprintf(f, "JOB_LOCAL_TASK_COUNT=%ld\n", job->j_local_task_count);
	rc = fprintf(f, "JOB_NCPUS=%ld\n", job->j_ncpus);
	rc = fprintf(f, "JOB_NAME=\"%s\"\n",
		     job->j_name ? job->j_name : "");

	pw = getpwuid(job->j_user_id);
	if (pw != NULL) {
		rc = fprintf(f, "JOB_USER=\"%s\"\n", pw->pw_name);
	} else {
		rc = fprintf(f, "JOB_USER=\"anonymous\"\n");
	}

	rc = fprintf(f, "JOB_PIDS=\"");
	int first = 1;
	LIST_FOREACH(jp, &job->j_pid_list, entry) {
		if (!first)
			rc = fprintf(f, ",");
		first = 0;
		rc = fprintf(f, "%ld", jp->j_pid);
	}
	rc = fprintf(f, "\"\n");

	fclose(f);
	return rc;
}

/*
 * Called by SLURM just before job start.
 */
int
slurm_spank_init(spank_t sh, int argc, char *argv[])
{
	spank_context_t context;
	spank_err_t err;
	char buf[512];
	job_info_t job;
	u_int job_id;
	u_int nnodes;

	if (spank_context() != S_CTX_REMOTE)
		return ESPANK_SUCCESS;

	/* If NNODES is 0, ignore this call */
	err = spank_get_item(sh, S_JOB_NNODES, &nnodes);
	if (nnodes == 0)
		return ESPANK_SUCCESS;

	err = spank_get_item(sh, S_JOB_ID, &job_id);
	job = find_job(job_id);
	if (!job) {
		job = calloc(1, sizeof *job);
		if (!job)
			return ESPANK_SUCCESS;
	}
	job->j_id = job_id;
	err = spank_get_item(sh, S_JOB_STEPID, &job->j_step_id);
	err = spank_get_item(sh, S_JOB_UID, &job->j_user_id);
	err = spank_get_item(sh, S_JOB_NNODES, &job->j_nnodes);
	err = spank_get_item(sh, S_JOB_LOCAL_TASK_COUNT, &job->j_local_task_count);
	err = spank_get_item(sh, S_JOB_NCPUS, &job->j_ncpus);
	err = spank_get_item(sh, S_JOB_ID, &job->j_id);
	err = spank_getenv(sh, "SLURM_JOB_NAME", buf, sizeof(buf));
	if (err == ESPANK_SUCCESS)
		job->j_name = strdup(buf);
	job->j_status = JOBINFO_JOB_STARTED;
	job->j_start = time(NULL);

	rbn_init(&job->j_rbn, &job->j_id);
	rbt_ins(&job_tree, &job->j_rbn);

	return ESPANK_SUCCESS;
}

int
slurm_spank_task_init(spank_t sh, int argc, char *argv[])
{
	pid_t pid;
	job_info_t job;
	u_int job_id;
	spank_err_t err;
	spank_context_t	context = spank_context();

	if (context != S_CTX_REMOTE)
		return ESPANK_SUCCESS;

	err = spank_get_item(sh, S_JOB_ID, &job_id);
	job = find_job(job_id);
	if (!job)
		return ESPANK_SUCCESS;

	err = spank_get_item(sh, S_JOB_STEPID, &job->j_step_id);
	err = spank_get_item(sh, S_TASK_PID, &pid);

	job_pid_t jp = malloc(sizeof *jp);
	if (!jp)
		return ESPANK_SUCCESS;

	jp->j_pid = pid;
	job->j_init_count += 1;
	LIST_INSERT_HEAD(&job->j_pid_list, jp, entry);

	if (job->j_init_count == job->j_local_task_count) {
		update_job_info(job);
	}
	return ESPANK_SUCCESS;
}

int
slurm_spank_task_init_privileged(spank_t sh, int argc, char *argv[])
{
	return slurm_spank_task_init(sh, argc, argv);
}

/**
 * local
 *
 *     In local context, the plugin is loaded by srun. (i.e. the
 *     "local" part of a parallel job).
 *
 * remote
 *
 *     In remote context, the plugin is loaded by
 *     slurmstepd. (i.e. the "remote" part of a parallel job).
 *
 * allocator
 *
 *     In allocator context, the plugin is loaded in one of the job
 *     allocation utilities sbatch or salloc.
 *
 * slurmd
 *
 *     In slurmd context, the plugin is loaded in the slurmd daemon
 *     itself. Note: Plugins loaded in slurmd context persist for the
 *     entire time slurmd is running, so if configuration is changed or
 *     plugins are updated, slurmd must be restarted for the changes to
 *     take effect.
 *
 * job_script
 *
 *     In the job_script context, plugins are loaded in the
 *     context of the job prolog or epilog. Note: Plugins are loaded
 *     in job_script context on each run on the job prolog or epilog,
 *     in a separate address space from plugins in slurmd
 *     context. This means there is no state shared between this
 *     context and other contexts, or even between one call to
 *     slurm_spank_job_prolog or slurm_spank_job_epilog and subsequent
 *     calls.
 */
/*
 * Called by SLURM just after job exit.
 */
int
slurm_spank_task_exit(spank_t sh, int argc, char *argv[])
{
	spank_err_t err;
	int val;
	u_int job_id;
	job_info_t job;

	if (spank_context() != S_CTX_REMOTE)
		return ESPANK_SUCCESS;

	err = spank_get_item(sh, S_JOB_ID, &job_id);
	if (err)
		return ESPANK_SUCCESS;

	job = find_job(job_id);
	if (!job)
		return ESPANK_SUCCESS;

	spank_get_item(sh, S_TASK_EXIT_STATUS, &val);
	job->j_exit_status = WEXITSTATUS(val);
	job->j_status = JOBINFO_JOB_EXITED;
	job->j_end = time(NULL);
 	update_job_info(job);
	free_job(job);
	return ESPANK_SUCCESS;
}

static int job_cmp(void *a, const void *b)
{
	return (*(u_int *)a - *(u_int *)b);
}

static void __attribute__ ((constructor)) jobinfo_init(void)
{
	rbt_init(&job_tree, job_cmp);
}

static void __attribute__ ((destructor)) jobinfo_term(void)
{
	struct rbn *rbn;
	job_info_t job;
	while (NULL != (rbn = rbt_min(&job_tree))) {
		job = container_of(rbn, struct job_info_s, j_rbn);
		free_job(job);
	}
}
