; ModuleID = 'source.spice'
source_filename = "source.spice"

%Nested = type { i8*, i1* }
%TestStruct = type { i32*, double, %Nested* }

@0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@1 = private unnamed_addr constant [23 x i8] c"Field1: %d, field2: %f\00", align 1
@2 = private unnamed_addr constant [15 x i8] c"Output: %s, %p\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %0 = alloca i32, align 4
  store i32 12, i32* %0, align 4
  %1 = load i32, i32* %0, align 4
  %input = alloca i32, align 4
  store i32 %1, i32* %input, align 4
  %2 = alloca i1, align 1
  store i1 true, i1* %2, align 1
  %3 = load i1, i1* %2, align 1
  %boolean = alloca i1, align 1
  store i1 %3, i1* %boolean, align 1
  %nestedInstance = alloca %Nested, align 8
  %4 = alloca i8*, align 8
  store i8* getelementptr inbounds ([13 x i8], [13 x i8]* @0, i32 0, i32 0), i8** %4, align 8
  %5 = load i8*, i8** %4, align 8
  %6 = getelementptr inbounds %Nested, %Nested* %nestedInstance, i32 0, i32 0
  store i8* %5, i8** %6, align 8
  %7 = alloca i1*, align 8
  store i1* %boolean, i1** %7, align 8
  %8 = load i1*, i1** %7, align 8
  %9 = getelementptr inbounds %Nested, %Nested* %nestedInstance, i32 0, i32 1
  store i1* %8, i1** %9, align 8
  %instance = alloca %TestStruct, align 8
  %10 = alloca i32*, align 8
  store i32* %input, i32** %10, align 8
  %11 = load i32*, i32** %10, align 8
  %12 = getelementptr inbounds %TestStruct, %TestStruct* %instance, i32 0, i32 0
  store i32* %11, i32** %12, align 8
  %13 = alloca double, align 8
  store double 4.634000e+01, double* %13, align 8
  %14 = load double, double* %13, align 8
  %15 = getelementptr inbounds %TestStruct, %TestStruct* %instance, i32 0, i32 1
  store double %14, double* %15, align 8
  %16 = alloca %Nested*, align 8
  store %Nested* %nestedInstance, %Nested** %16, align 8
  %17 = load %Nested*, %Nested** %16, align 8
  %18 = getelementptr inbounds %TestStruct, %TestStruct* %instance, i32 0, i32 2
  store %Nested* %17, %Nested** %18, align 8
  %19 = load %TestStruct, %TestStruct* %instance, align 8
  %instance1 = alloca %TestStruct, align 8
  store %TestStruct %19, %TestStruct* %instance1, align 8
  %20 = getelementptr inbounds %TestStruct, %TestStruct* %instance, i32 0, i32 2
  %21 = load %Nested*, %Nested** %20, align 8
  %22 = getelementptr inbounds %Nested, %Nested* %21, i32 0, i32 1
  %23 = load i1*, i1** %22, align 8
  %24 = load i1, i1* %23, align 1
  %25 = zext i1 %24 to i32
  %26 = getelementptr inbounds %TestStruct, %TestStruct* %instance1, i32 0, i32 1
  %27 = load double, double* %26, align 8
  %28 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @1, i32 0, i32 0), i32 %25, double %27)
  %29 = getelementptr inbounds %TestStruct, %TestStruct* %instance1, i32 0, i32 2
  %30 = load %Nested*, %Nested** %29, align 8
  %31 = getelementptr inbounds %Nested, %Nested* %30, i32 0, i32 0
  %32 = load i8*, i8** %31, align 8
  %33 = getelementptr inbounds %TestStruct, %TestStruct* %instance, i32 0, i32 0
  %34 = load i32*, i32** %33, align 8
  %35 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @2, i32 0, i32 0), i8* %32, i32* %34)
  %36 = load i32, i32* %result, align 4
  ret i32 %36
}
