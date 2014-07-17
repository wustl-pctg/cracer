#define AUTO 257
#define DOUBLE 258
#define INT 259
#define STRUCT 260
#define BREAK 261
#define ELSE 262
#define LONG 263
#define SWITCH 264
#define CASE 265
#define ENUM 266
#define REGISTER 267
#define TYPEDEF 268
#define CHAR 269
#define EXTERN 270
#define RETURN 271
#define UNION 272
#define CONST 273
#define FLOAT 274
#define SHORT 275
#define UNSIGNED 276
#define CONTINUE 277
#define FOR 278
#define SIGNED 279
#define VOID 280
#define DEFAULT 281
#define GOTO 282
#define SIZEOF 283
#define VOLATILE 284
#define DO 285
#define IF 286
#define STATIC 287
#define WHILE 288
#define UPLUS 289
#define UMINUS 290
#define INDIR 291
#define ADDRESS 292
#define POSTINC 293
#define POSTDEC 294
#define PREINC 295
#define PREDEC 296
#define BOGUS 297
#define IDENTIFIER 298
#define STRINGliteral 299
#define FLOATINGconstant 300
#define INTEGERconstant 301
#define OCTALconstant 302
#define HEXconstant 303
#define WIDECHARconstant 304
#define CHARACTERconstant 305
#define COMPLEX 306
#define REAL 307
#define IMAG 308
#define TYPEDEFname 309
#define ARROW 310
#define ICR 311
#define DECR 312
#define LS 313
#define RS 314
#define LE 315
#define GE 316
#define EQ 317
#define NE 318
#define ANDAND 319
#define OROR 320
#define ELLIPSIS 321
#define MULTassign 322
#define DIVassign 323
#define MODassign 324
#define PLUSassign 325
#define MINUSassign 326
#define LSassign 327
#define RSassign 328
#define ANDassign 329
#define ERassign 330
#define ORassign 331
#define INLINE 332
#define ATTRIBUTE 333
#define EXTENSION 334
#define RESTRICT 335
#define ALIGNOF 336
#define TYPEOF 337
#define DECLSPEC_EXPORT 338
#define DECLSPEC_IMPORT 339
#define BUILTIN_VA_ARG 340
#define DESIGNATED_INITIALIZER 341
#define ARRAY_LABELED_INITIALIZER 342
#define NOOUTPUT 343
#define CILKONLY 344
#define CILKSAFE 345
#define WILDCARD_T 346
#define WILDCARD_E 347
#define WILDCARD_D 348
#define ASM 349
#define PROCEDURE 350
#define SPAWN 351
#define SYNC 352
#define INLET 353
#define ABORT 354
#define XBEGIN 355
#define XEND 356
#define SYNCHED 357
#define SHAREDVAR 358
#define PRIVATEVAR 359
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
    Node      *n;
    List      *L;

  /* tq: type qualifiers */
    struct {
        TypeQual   tq;
	Coord      coord;   /* coordinates where type quals began */ 
    } tq;

  /* tok: token coordinates */
    Coord tok;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;
