; ModuleID = 'source.spice'
source_filename = "source.spice"

@0 = private unnamed_addr constant [6 x i8] c"Pizza\00", align 1
@1 = private unnamed_addr constant [31 x i8] c"Pointer address: %p, value: %s\00", align 1
@2 = private unnamed_addr constant [19 x i8] c"Restored value: %s\00", align 1
@3 = private unnamed_addr constant [27 x i8] c"Restored value address: %p\00", align 1

declare i32 @printf(i8*, ...)

declare i8* @malloc(i32)

declare void @free(i8*)

declare i8* @memcpy(i8*, i8*, i32)

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %food = alloca i8*, align 8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @0, i32 0, i32 0), i8** %food, align 8
  %ptr = alloca i8**, align 8
  store i8** %food, i8*** %ptr, align 8
  %0 = load i8**, i8*** %ptr, align 8
  %1 = load i8**, i8*** %ptr, align 8
  %2 = load i8*, i8** %1, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @1, i32 0, i32 0), i8** %0, i8* %2)
  %4 = load i8**, i8*** %ptr, align 8
  %5 = load i8*, i8** %4, align 8
  %restoredFood = alloca i8*, align 8
  store i8* %5, i8** %restoredFood, align 8
  %6 = load i8*, i8** %restoredFood, align 8
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @2, i32 0, i32 0), i8* %6)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @3, i32 0, i32 0), i8** %restoredFood)
  %9 = load i32, i32* %result, align 4
  ret i32 %9
}
