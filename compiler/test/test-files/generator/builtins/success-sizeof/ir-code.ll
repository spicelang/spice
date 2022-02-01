; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [17 x i8] c"Size of int: %d\0A\00", align 1
@1 = private unnamed_addr constant [19 x i8] c"Size of short: %d\0A\00", align 1
@2 = private unnamed_addr constant [18 x i8] c"Size of byte: %d\0A\00", align 1
@3 = private unnamed_addr constant [18 x i8] c"Size of char: %d\0A\00", align 1
@4 = private unnamed_addr constant [20 x i8] c"Size of string: %d\0A\00", align 1
@5 = private unnamed_addr constant [13 x i8] c"Hello Spice!\00", align 1
@6 = private unnamed_addr constant [18 x i8] c"Size of bool: %d\0A\00", align 1
@intArray = dso_local constant [7 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7]
@7 = private unnamed_addr constant [19 x i8] c"Size of int[]: %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i16, align 2
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i8, align 1
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i8, align 1
  %10 = alloca i32, align 4
  %11 = alloca i8*, align 8
  %12 = alloca i32, align 4
  %13 = alloca i1, align 1
  %14 = alloca i32, align 4
  %intArray = alloca [7 x i32], align 4
  %15 = alloca [7 x i32], align 4
  %16 = alloca i32, align 4
  %17 = alloca i32, align 4
  %18 = alloca i32, align 4
  %19 = alloca i32, align 4
  %20 = alloca i32, align 4
  %21 = alloca i32, align 4
  %22 = alloca i32, align 4
  %23 = alloca i32, align 4
  store i32 0, i32* %result, align 4
  store i32 353, i32* %0, align 4
  %24 = load i32, i32* %0, align 4
  store i32 32, i32* %1, align 4
  %25 = load i32, i32* %1, align 4
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @0, i32 0, i32 0), i32 %25)
  store i32 35, i32* %2, align 4
  %27 = load i32, i32* %2, align 4
  %28 = trunc i32 %27 to i16
  store i16 %28, i16* %3, align 2
  %29 = load i16, i16* %3, align 2
  store i32 16, i32* %4, align 4
  %30 = load i32, i32* %4, align 4
  %31 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @1, i32 0, i32 0), i32 %30)
  store i32 13, i32* %5, align 4
  %32 = load i32, i32* %5, align 4
  %33 = trunc i32 %32 to i8
  store i8 %33, i8* %6, align 1
  %34 = load i8, i8* %6, align 1
  store i32 8, i32* %7, align 4
  %35 = load i32, i32* %7, align 4
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @2, i32 0, i32 0), i32 %35)
  store i32 65, i32* %8, align 4
  %37 = load i32, i32* %8, align 4
  %38 = trunc i32 %37 to i8
  store i8 %38, i8* %9, align 1
  %39 = load i8, i8* %9, align 1
  store i32 8, i32* %10, align 4
  %40 = load i32, i32* %10, align 4
  %41 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @3, i32 0, i32 0), i32 %40)
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @5, i32 0, i32 0), i8** %11, align 8
  %42 = load i8*, i8** %11, align 8
  store i32 0, i32* %12, align 4
  %43 = load i32, i32* %12, align 4
  %44 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @4, i32 0, i32 0), i32 %43)
  store i1 false, i1* %13, align 1
  %45 = load i1, i1* %13, align 1
  store i32 1, i32* %14, align 4
  %46 = load i32, i32* %14, align 4
  %47 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @6, i32 0, i32 0), i32 %46)
  store i32 1, i32* %16, align 4
  store i32 2, i32* %17, align 4
  store i32 3, i32* %18, align 4
  store i32 4, i32* %19, align 4
  store i32 5, i32* %20, align 4
  store i32 6, i32* %21, align 4
  store i32 7, i32* %22, align 4
  %48 = load [7 x i32], [7 x i32]* @intArray, align 4
  store [7 x i32] %48, [7 x i32]* %intArray, align 4
  %49 = load [7 x i32], [7 x i32]* %intArray, align 4
  store i32 7, i32* %23, align 4
  %50 = load i32, i32* %23, align 4
  %51 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @7, i32 0, i32 0), i32 %50)
  %52 = load i32, i32* %result, align 4
  ret i32 %52
}
