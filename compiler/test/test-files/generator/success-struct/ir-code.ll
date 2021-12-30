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
  %7 = getelementptr inbounds i1, i1* %boolean, i32 0
  %8 = alloca i1*, align 8
  store i1* %boolean, i1** %8, align 8
  %9 = load i1*, i1** %8, align 8
  %10 = getelementptr inbounds %Nested, %Nested* %nestedInstance, i32 0, i32 1
  store i1* %9, i1** %10, align 8
  %instance = alloca %TestStruct, align 8
  %11 = getelementptr inbounds i32, i32* %input, i32 0
  %12 = alloca i32*, align 8
  store i32* %input, i32** %12, align 8
  %13 = load i32*, i32** %12, align 8
  %14 = getelementptr inbounds %TestStruct, %TestStruct* %instance, i32 0, i32 0
  store i32* %13, i32** %14, align 8
  %15 = alloca double, align 8
  store double 4.634000e+01, double* %15, align 8
  %16 = load double, double* %15, align 8
  %17 = getelementptr inbounds %TestStruct, %TestStruct* %instance, i32 0, i32 1
  store double %16, double* %17, align 8
  %18 = getelementptr inbounds %Nested, %Nested* %nestedInstance, i32 0
  %19 = alloca %Nested*, align 8
  store %Nested* %nestedInstance, %Nested** %19, align 8
  %20 = load %Nested*, %Nested** %19, align 8
  %21 = getelementptr inbounds %TestStruct, %TestStruct* %instance, i32 0, i32 2
  store %Nested* %20, %Nested** %21, align 8
  %22 = getelementptr inbounds %TestStruct, %TestStruct* %instance, i32 0
  %23 = load %TestStruct, %TestStruct* %22, align 8
  %instance1 = alloca %TestStruct, align 8
  store %TestStruct %23, %TestStruct* %instance1, align 8
  %24 = getelementptr inbounds %TestStruct, %TestStruct* %instance, i32 0, i32 2
  %25 = load %Nested*, %Nested** %24, align 8
  %26 = getelementptr inbounds %Nested, %Nested* %25, i32 0, i32 1
  %27 = load i1*, i1** %26, align 8
  %28 = load i1, i1* %27, align 1
  %29 = zext i1 %28 to i32
  %30 = getelementptr inbounds %TestStruct, %TestStruct* %instance1, i32 0, i32 1
  %31 = load double, double* %30, align 8
  %32 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @1, i32 0, i32 0), i32 %29, double %31)
  %33 = getelementptr inbounds %TestStruct, %TestStruct* %instance1, i32 0, i32 2
  %34 = load %Nested*, %Nested** %33, align 8
  %35 = getelementptr inbounds %Nested, %Nested* %34, i32 0, i32 0
  %36 = load i8*, i8** %35, align 8
  %37 = getelementptr inbounds %TestStruct, %TestStruct* %instance, i32 0, i32 0
  %38 = load i32*, i32** %37, align 8
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @2, i32 0, i32 0), i8* %36, i32* %38)
  %40 = load i32, i32* %result, align 4
  ret i32 %40
}
