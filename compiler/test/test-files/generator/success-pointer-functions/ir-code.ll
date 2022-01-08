; ModuleID = 'source.spice'
source_filename = "source.spice"

%Person = type { i8*, i8*, i32 }

@0 = private unnamed_addr constant [5 x i8] c"Mike\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"Miller\00", align 1
@2 = private unnamed_addr constant [15 x i8] c"Person: %s, %s\00", align 1
@3 = private unnamed_addr constant [24 x i8] c"Age before birthday: %d\00", align 1
@4 = private unnamed_addr constant [23 x i8] c"Age after birthday: %d\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define void @"birthday(struct(Person)*)"(%Person* %0) #0 {
entry:
  %person = alloca %Person*, align 8
  store %Person* %0, %Person** %person, align 8
  %1 = load %Person*, %Person** %person, align 8
  %2 = getelementptr inbounds %Person, %Person* %1, i32 0, i32 2
  %3 = load i32, i32* %2, align 4
  %4 = add i32 %3, 1
  store i32 %4, i32* %2, align 4
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  store i32 0, i32* %result, align 4
  %mike = alloca %Person, align 8
  %0 = alloca i8*, align 8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i32 0, i32 0), i8** %0, align 8
  %1 = load i8*, i8** %0, align 8
  %2 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 0
  store i8* %1, i8** %2, align 8
  %3 = alloca i8*, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @1, i32 0, i32 0), i8** %3, align 8
  %4 = load i8*, i8** %3, align 8
  %5 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 1
  store i8* %4, i8** %5, align 8
  %6 = alloca i32, align 4
  store i32 32, i32* %6, align 4
  %7 = load i32, i32* %6, align 4
  %8 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 2
  store i32 %7, i32* %8, align 4
  %9 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 1
  %10 = load i8*, i8** %9, align 8
  %11 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 0
  %12 = load i8*, i8** %11, align 8
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @2, i32 0, i32 0), i8* %10, i8* %12)
  %14 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 2
  %15 = load i32, i32* %14, align 4
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @3, i32 0, i32 0), i32 %15)
  %17 = alloca %Person*, align 8
  store %Person* %mike, %Person** %17, align 8
  %18 = load %Person*, %Person** %17, align 8
  call void @"birthday(struct(Person)*)"(%Person* %18)
  %19 = alloca i1, align 1
  store i1 true, i1* %19, align 1
  %20 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 2
  %21 = load i32, i32* %20, align 4
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @4, i32 0, i32 0), i32 %21)
  %23 = load i32, i32* %result, align 4
  ret i32 %23
}

attributes #0 = { nounwind }
