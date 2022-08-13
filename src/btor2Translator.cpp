#include "btor2parser/btor2parser.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *input_name;
static const char *output_name;

static bool unroll_btor = false;
static int unroll_time = 0;
static bool translate_btor = false;
static bool verbosity = false;

int get_number(char *s)
{
    int num = 0;
    char *c = s;
    while (*c != '\0')
    {
        if ('0' <= *c && *c <= '9')
        {
            num = num * 10 + *c - '0';
            c++;
        }
        else
        {
            fprintf(stderr, "*** trans: wrong number format '%s'\n", s);
            exit(1);
        }
    }
    return num;
}

int process_parameters(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {
            fprintf(stdout, "usage: btorTrans [-u <unroll_time>] [-v] [-t] [-o <output_file>] [<btorfile>]\n");
            exit(0);
        }
        else if (!strcmp(argv[i], "-u") || !strcmp(argv[i], "--unroll"))
        {
            if (i + 1 < argc)
            {
                unroll_btor = true;
                unroll_time = get_number(argv[i + 1]);
                i++;
            }
            else
            {
                fprintf(stderr, "*** trans: miss unroll time\n");
                exit(1);
            }
        }
        else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--trans"))
        {
            translate_btor = true;
        }
        else if (!strcmp(argv[i], "-o"))
        {
            if (i + 1 < argc)
            {
                output_name = argv[i + 1];
                i++;
            }
            else
            {
                fprintf(stderr, "*** trans: miss output file name\n");
                exit(1);
            }
        }
        else if (!strcmp(argv[i], "-v"))
        {
            verbosity = true;
        }
        else if (argv[i][0] == '-')
        {
            fprintf(
                stderr, "*** trans: invalid option '%s' (try '-h')\n", argv[i]);
            exit(1);
        }
        else
        {
            if (input_name)
            {
                fprintf(stderr, "*** trans: too many inputs (try '-h')\n");
                exit(1);
            }
            else
            {
                input_name = argv[i];
            }
        }
    }
    return 0;
}

Btor2Parser *load_btor_file(const char *file_name)
{
    Btor2Parser *reader;
    const char *err;
    bool close_input = false;
    FILE *input_file;

    if (!file_name)
    {
        input_file = stdin;
        assert(!close_input);
        file_name = "<stdin>";
    }
    else
    {
        input_file = fopen(file_name, "r");
        if (!input_file)
        {
            fprintf(
                stderr, "*** can not open '%s' for reading\n", file_name);
            exit(1);
        }
        close_input = 1;
    }
    reader = btor2parser_new();
    if (!btor2parser_read_lines(reader, input_file))
    {
        err = btor2parser_error(reader);
        assert(err);
        fprintf(stderr, "*** parse error in '%s' %s\n", file_name, err);
        btor2parser_delete(reader);
        if (close_input)
            fclose(input_file);
        exit(1);
    }
    if (close_input)
        fclose(input_file);
    if (verbosity)
    {
        fprintf(stderr, "; [trans] finished parsing '%s'\n", file_name);
        fflush(stderr);
    }
    return reader;
}

int dump_btor(Btor2Parser *reader, const char *file_name)
{
    Btor2LineIterator it;
    Btor2Line *l;

    FILE *output_file;
    bool close_output = false;
    if (!file_name)
    {
        output_file = stdout;
        assert(!close_output);
        file_name = "<stdout>";
    }
    else
    {
        output_file = fopen(file_name, "w");
        if (!output_file)
        {
            fprintf(
                stderr, "*** can not open '%s' for writing\n", file_name);
            exit(1);
        }
        close_output = 1;
    }
    if (verbosity)
    {
        fprintf(stderr, "; [trans] starting to dump BTOR model to '<stdout>'\n");
        fflush(stderr);
    }

    it = btor2parser_iter_init(reader);
    while ((l = btor2parser_iter_next(&it)))
    {
        fprintf(output_file, "%" PRId64 " %s", l->id, l->name);
        if (l->tag == BTOR2_TAG_sort)
        {
            fprintf(output_file, " %s", l->sort.name);
            switch (l->sort.tag)
            {
            case BTOR2_TAG_SORT_bitvec:
                fprintf(output_file, " %u", l->sort.bitvec.width);
                break;
            case BTOR2_TAG_SORT_array:
                fprintf(output_file, " %" PRId64 " %" PRId64, l->sort.array.index, l->sort.array.element);
                break;
            default:
                assert(0);
                fprintf(stderr, "*** trans: invalid sort encountered\n");
                exit(1);
            }
        }
        else if (l->sort.id)
            fprintf(output_file, " %" PRId64, l->sort.id);
        for (int j = 0; j < l->nargs; j++)
            fprintf(output_file, " %" PRId64, l->args[j]);
        if (l->tag == BTOR2_TAG_slice)
            fprintf(output_file, " %" PRId64 " %" PRId64, l->args[1], l->args[2]);
        if (l->tag == BTOR2_TAG_sext || l->tag == BTOR2_TAG_uext)
            fprintf(output_file, " %" PRId64, l->args[1]);
        if (l->constant)
            fprintf(output_file, " %s", l->constant);
        if (l->symbol)
            fprintf(output_file, " %s", l->symbol);
        fputc('\n', output_file);
    }

    if (verbosity)
    {
        fprintf(stderr, "; [trans] finished dumping BTOR model to '<stdout>'\n");
        fflush(stderr);
    }
    if (close_output)
        fclose(output_file);
    if (verbosity)
    {
        fprintf(stderr, "; [trans] finished dumping to '%s'\n", file_name);
        fflush(stderr);
    }
    return 0;
}

int main(int argc, char **argv)
{
    process_parameters(argc, argv);

    Btor2Parser *reader;
    reader = load_btor_file(input_name);

    if (unroll_btor)
    {
        // do some unroll
    }

    if (translate_btor)
    {
        // do some translate
    }

    // dump
    if (translate_btor)
    {
        // dump formula
    }
    else
    {
        dump_btor(reader, output_name);
    }

    btor2parser_delete(reader);

    return 0;
}
