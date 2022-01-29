; ModuleID = 'source.spice'
source_filename = "source.spice"

%Person = type { i8*, i8*, i32 }

@0 = private unnamed_addr constant [5 x i8] c"Mike\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"Miller\00", align 1
@2 = private unnamed_addr constant [16 x i8] c"Person: %s, %s\0A\00", align 1
@3 = private unnamed_addr constant [25 x i8] c"Age before birthday: %d\0A\00", align 1
@4 = private unnamed_addr constant [24 x i8] c"Age after birthday: %d\0A\00", align 1

declare i32 @printf(i8*, ...)

; Function Attrs: nounwind
define void @"birthday(struct(Person)*)"(%Person* %0) #0 {
entry:
  %person = alloca %Person*, align 8
  store %Person* %0, %Person** %person, align 8
  %1 = load %Person*, %Person** %person, align 8
  %2 = load %Person*, %Person** %person, align 8
  %3 = getelementptr inbounds %Person, %Person* %2, i32 0, i32 2
  %4 = load i32, i32* %3, align 4
  %5 = add i32 %4, 1
  store i32 %5, i32* %3, align 4
  ret void
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %mike = alloca %Person, align 8
  %Person = alloca %Person, align 8
  %0 = alloca i8*, align 8
  %1 = alloca i8*, align 8
  %2 = alloca i32, align 4
  %3 = alloca %Person*, align 8
  %4 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @0, i32 0, i32 0), i8** %0, align 8
  %5 = load i8*, i8** %0, align 8
  %6 = getelementptr inbounds %Person, %Person* %Person, i32 0, i32 0
  store i8* %5, i8** %6, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @1, i32 0, i32 0), i8** %1, align 8
  %7 = load i8*, i8** %1, align 8
  %8 = getelementptr inbounds %Person, %Person* %Person, i32 0, i32 1
  store i8* %7, i8** %8, align 8
  store i32 32, i32* %2, align 4
  %9 = load i32, i32* %2, align 4
  %10 = getelementptr inbounds %Person, %Person* %Person, i32 0, i32 2
  store i32 %9, i32* %10, align 4
  %11 = load %Person, %Person* %Person, align 8
  store %Person %11, %Person* %mike, align 8
  %12 = load %Person, %Person* %mike, align 8
  %13 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 1
  %14 = load i8*, i8** %13, align 8
  %15 = load %Person, %Person* %mike, align 8
  %16 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 0
  %17 = load i8*, i8** %16, align 8
  %18 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @2, i32 0, i32 0), i8* %14, i8* %17)
  %19 = load %Person, %Person* %mike, align 8
  %20 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 2
  %21 = load i32, i32* %20, align 4
  %22 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @3, i32 0, i32 0), i32 %21)
  %23 = load %Person, %Person* %mike, align 8
  store %Person* %mike, %Person** %3, align 8
  store %Person* %mike, %Person** %3, align 8
  %24 = load %Person*, %Person** %3, align 8
  call void @"birthday(struct(Person)*)"(%Person* %24)
  store i1 true, i1* %4, align 1
  %25 = load %Person, %Person* %mike, align 8
  %26 = getelementptr inbounds %Person, %Person* %mike, i32 0, i32 2
  %27 = load i32, i32* %26, align 4
  %28 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @4, i32 0, i32 0), i32 %27)
  %29 = load i32, i32* %result, align 4
  ret i32 %29
}

attributes #0 = { nounwind }
