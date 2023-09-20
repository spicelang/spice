; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.TestStruct = type { i32, ptr, double, i1 }

@anon.string.0 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@printf.str.0 = private unnamed_addr constant [26 x i8] c"Default value of int: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [29 x i8] c"Default value of string: %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [29 x i8] c"Default value of double: %f\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [27 x i8] c"Default value of bool: %d\0A\00", align 1

; Function Attrs: norecurse
define private void @_ZN10TestStruct4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds %struct.TestStruct, ptr %2, i32 0, i32 0
  store i32 123, ptr %3, align 4
  %4 = getelementptr inbounds %struct.TestStruct, ptr %2, i32 0, i32 1
  store ptr @anon.string.0, ptr %4, align 8
  %5 = getelementptr inbounds %struct.TestStruct, ptr %2, i32 0, i32 2
  store double 1.230000e+00, ptr %5, align 8
  %6 = getelementptr inbounds %struct.TestStruct, ptr %2, i32 0, i32 3
  store i1 true, ptr %6, align 1
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %ts = alloca %struct.TestStruct, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN10TestStruct4ctorEv(ptr %ts)
  %i_addr = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 0
  %1 = load i32, ptr %i_addr, align 4
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %1)
  %s_addr = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 1
  %3 = load ptr, ptr %s_addr, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %3)
  %d_addr = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 2
  %5 = load double, ptr %d_addr, align 8
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, double %5)
  %b_addr = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 3
  %7 = load i1, ptr %b_addr, align 1
  %8 = zext i1 %7 to i32
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 %8)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #2

attributes #0 = { norecurse }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
