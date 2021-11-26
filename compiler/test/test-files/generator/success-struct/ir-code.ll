; ModuleID = 'source.spice'
source_filename = "source.spice"

%TestStruct = type { i32*, double }

@0 = private unnamed_addr constant [23 x i8] c"Field1: %p, field2: %f\00", align 1

declare i32 @printf(i8*, ...)

declare i8* @malloc(i32)

declare void @free(i8*)

declare i8* @memcpy(i8*, i8*, i32)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %input = alloca i32, align 4
  store i32 12, i32* %input, align 4
  %0 = alloca %TestStruct, align 8
  %1 = getelementptr inbounds %TestStruct, %TestStruct* %0, i32 0, i32 0
  store i32* %input, i32** %1, align 8
  %2 = getelementptr inbounds %TestStruct, %TestStruct* %0, i32 0, i32 1
  store double 4.634000e+01, double* %2, align 8
  %TestStruct = load %TestStruct, %TestStruct* %0, align 8
  %instance = alloca %TestStruct, align 8
  store %TestStruct %TestStruct, %TestStruct* %instance, align 8
  %3 = load %TestStruct, %TestStruct* %instance, align 8
  %instance1 = alloca %TestStruct, align 8
  store %TestStruct %3, %TestStruct* %instance1, align 8
  %4 = getelementptr inbounds %TestStruct, %TestStruct* %instance1, i32 0, i32 0
  %5 = load i32*, i32** %4, align 8
  %6 = load i32, i32* %5, align 4
  %7 = getelementptr inbounds %TestStruct, %TestStruct* %instance1, i32 0, i32 1
  %8 = load double, double* %7, align 8
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @0, i32 0, i32 0), i32 %6, double %8)
  %10 = load i32, i32* %result, align 4
  ret i32 %10
}
