; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Nested = type { ptr, ptr }
%struct.TestStruct = type { ptr, double, ptr }

@anon.string.0 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@printf.str.0 = private unnamed_addr constant [24 x i8] c"Field1: %d, field2: %f\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Output: %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %input = alloca i32, align 4
  %boolean = alloca i1, align 1
  %nestedInstance = alloca %struct.Nested, align 8
  %instance = alloca %struct.TestStruct, align 8
  %instance1 = alloca %struct.TestStruct, align 8
  store i32 0, ptr %result, align 4
  store i32 12, ptr %input, align 4
  store i1 true, ptr %boolean, align 1
  store ptr @anon.string.0, ptr %nestedInstance, align 8
  %1 = getelementptr inbounds %struct.Nested, ptr %nestedInstance, i32 0, i32 1
  store ptr %boolean, ptr %1, align 8
  store ptr %input, ptr %instance, align 8
  %2 = getelementptr inbounds %struct.TestStruct, ptr %instance, i32 0, i32 1
  store double 4.634000e+01, ptr %2, align 8
  %3 = getelementptr inbounds %struct.TestStruct, ptr %instance, i32 0, i32 2
  store ptr %nestedInstance, ptr %3, align 8
  %4 = load %struct.TestStruct, ptr %instance, align 8
  store %struct.TestStruct %4, ptr %instance1, align 8
  %nested_addr = getelementptr inbounds %struct.TestStruct, ptr %instance, i32 0, i32 2
  %5 = load ptr, ptr %nested_addr, align 8
  %nested2_addr = getelementptr inbounds %struct.Nested, ptr %5, i32 0, i32 1
  %6 = load ptr, ptr %nested2_addr, align 8
  %7 = load i1, ptr %6, align 1
  %8 = zext i1 %7 to i32
  %field2_addr = getelementptr inbounds %struct.TestStruct, ptr %instance1, i32 0, i32 1
  %9 = load double, ptr %field2_addr, align 8
  %10 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %8, double %9)
  %nested_addr1 = getelementptr inbounds %struct.TestStruct, ptr %instance1, i32 0, i32 2
  %11 = load ptr, ptr %nested_addr1, align 8
  %nested1_addr = getelementptr inbounds %struct.Nested, ptr %11, i32 0, i32 0
  %12 = load ptr, ptr %nested1_addr, align 8
  %13 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %12)
  %14 = load i32, ptr %result, align 4
  ret i32 %14
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
