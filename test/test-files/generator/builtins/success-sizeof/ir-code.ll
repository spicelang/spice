; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s_Struct_int_bool_string = type { i32, i1, i8* }

@0 = private unnamed_addr constant [20 x i8] c"Size of double: %d\0A\00", align 1
@1 = private unnamed_addr constant [17 x i8] c"Size of int: %d\0A\00", align 1
@2 = private unnamed_addr constant [19 x i8] c"Size of short: %d\0A\00", align 1
@3 = private unnamed_addr constant [18 x i8] c"Size of long: %d\0A\00", align 1
@4 = private unnamed_addr constant [18 x i8] c"Size of byte: %d\0A\00", align 1
@5 = private unnamed_addr constant [18 x i8] c"Size of char: %d\0A\00", align 1
@6 = private unnamed_addr constant [20 x i8] c"Size of string: %d\0A\00", align 1
@7 = private unnamed_addr constant [13 x i8] c"Hello Spice!\00", align 1
@8 = private unnamed_addr constant [18 x i8] c"Size of bool: %d\0A\00", align 1
@9 = private unnamed_addr constant [19 x i8] c"Size of int[]: %d\0A\00", align 1
@anonymous.0 = constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7]
@10 = private unnamed_addr constant [18 x i8] c"Size of int*: %d\0A\00", align 1
@11 = private unnamed_addr constant [29 x i8] c"Size of struct instance: %d\0A\00", align 1
@12 = private unnamed_addr constant [15 x i8] c"This is a test\00", align 1

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca double, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i16, align 2
  %5 = alloca i32, align 4
  %6 = alloca i64, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i8, align 1
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i8, align 1
  %13 = alloca i32, align 4
  %14 = alloca i8*, align 8
  %15 = alloca i32, align 4
  %16 = alloca i1, align 1
  %17 = alloca i32, align 4
  %18 = alloca i32, align 4
  %19 = alloca i32, align 4
  %20 = alloca i32, align 4
  %21 = alloca i32, align 4
  %22 = alloca i32, align 4
  %23 = alloca i32, align 4
  %24 = alloca i32, align 4
  %25 = alloca i32, align 4
  %intVariable = alloca i32, align 4
  %26 = alloca i32, align 4
  %27 = alloca i32*, align 8
  %28 = alloca i32, align 4
  %29 = alloca %_s_Struct_int_bool_string, align 8
  %30 = alloca i32, align 4
  %31 = alloca i1, align 1
  %32 = alloca i8*, align 8
  %33 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store double 0x40335992641B328B, double* %0, align 8
  %34 = load double, double* %0, align 8
  %35 = fmul double %34, -1.000000e+00
  store double %35, double* %0, align 8
  %36 = load double, double* %0, align 8
  store i32 64, i32* %1, align 4
  %37 = load i32, i32* %1, align 4
  %38 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @0, i32 0, i32 0), i32 %37)
  store i32 353, i32* %2, align 4
  %39 = load i32, i32* %2, align 4
  store i32 32, i32* %3, align 4
  %40 = load i32, i32* %3, align 4
  %41 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @1, i32 0, i32 0), i32 %40)
  store i16 35, i16* %4, align 2
  %42 = load i16, i16* %4, align 2
  store i32 16, i32* %5, align 4
  %43 = load i32, i32* %5, align 4
  %44 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @2, i32 0, i32 0), i32 %43)
  store i64 9223372036854775807, i64* %6, align 8
  %45 = load i64, i64* %6, align 8
  store i32 64, i32* %7, align 4
  %46 = load i32, i32* %7, align 4
  %47 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @3, i32 0, i32 0), i32 %46)
  store i32 13, i32* %8, align 4
  %48 = load i32, i32* %8, align 4
  %49 = trunc i32 %48 to i8
  store i8 %49, i8* %9, align 1
  %50 = load i8, i8* %9, align 1
  store i32 8, i32* %10, align 4
  %51 = load i32, i32* %10, align 4
  %52 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @4, i32 0, i32 0), i32 %51)
  store i32 65, i32* %11, align 4
  %53 = load i32, i32* %11, align 4
  %54 = trunc i32 %53 to i8
  store i8 %54, i8* %12, align 1
  %55 = load i8, i8* %12, align 1
  store i32 8, i32* %13, align 4
  %56 = load i32, i32* %13, align 4
  %57 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @5, i32 0, i32 0), i32 %56)
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @7, i32 0, i32 0), i8** %14, align 8
  %58 = load i8*, i8** %14, align 8
  store i32 8, i32* %15, align 4
  %59 = load i32, i32* %15, align 4
  %60 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @6, i32 0, i32 0), i32 %59)
  store i1 false, i1* %16, align 1
  %61 = load i1, i1* %16, align 1
  store i32 1, i32* %17, align 4
  %62 = load i32, i32* %17, align 4
  %63 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @8, i32 0, i32 0), i32 %62)
  store i32 1, i32* %18, align 4
  store i32 2, i32* %19, align 4
  store i32 3, i32* %20, align 4
  store i32 4, i32* %21, align 4
  store i32 5, i32* %22, align 4
  store i32 6, i32* %23, align 4
  store i32 7, i32* %24, align 4
  %64 = load [7 x i32], [7 x i32]* @anonymous.0, align 4
  store i32 7, i32* %25, align 4
  %65 = load i32, i32* %25, align 4
  %66 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @9, i32 0, i32 0), i32 %65)
  store i32 123, i32* %26, align 4
  %67 = load i32, i32* %26, align 4
  store i32 %67, i32* %intVariable, align 4
  %68 = load i32, i32* %intVariable, align 4
  store i32* %intVariable, i32** %27, align 8
  store i32* %intVariable, i32** %27, align 8
  %69 = load i32*, i32** %27, align 8
  store i32 32, i32* %28, align 4
  %70 = load i32, i32* %28, align 4
  %71 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @10, i32 0, i32 0), i32 %70)
  store i32 5, i32* %30, align 4
  %72 = load i32, i32* %30, align 4
  %73 = getelementptr inbounds %_s_Struct_int_bool_string, %_s_Struct_int_bool_string* %29, i32 0, i32 0
  store i32 %72, i32* %73, align 4
  store i1 true, i1* %31, align 1
  %74 = load i1, i1* %31, align 1
  %75 = getelementptr inbounds %_s_Struct_int_bool_string, %_s_Struct_int_bool_string* %29, i32 0, i32 1
  store i1 %74, i1* %75, align 1
  store i8* getelementptr inbounds ([15 x i8], [15 x i8]* @12, i32 0, i32 0), i8** %32, align 8
  %76 = load i8*, i8** %32, align 8
  %77 = getelementptr inbounds %_s_Struct_int_bool_string, %_s_Struct_int_bool_string* %29, i32 0, i32 2
  store i8* %76, i8** %77, align 8
  %78 = load %_s_Struct_int_bool_string, %_s_Struct_int_bool_string* %29, align 8
  store i32 3, i32* %33, align 4
  %79 = load i32, i32* %33, align 4
  %80 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @11, i32 0, i32 0), i32 %79)
  %81 = load i32, i32* %result, align 4
  ret i32 %81
}

declare i32 @printf(i8*, ...)
