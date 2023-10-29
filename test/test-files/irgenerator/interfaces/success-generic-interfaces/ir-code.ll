; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Person = type { %interface.Compareable, ptr, ptr, i32 }
%interface.Compareable = type { ptr }

$_ZTS11CompareableIlE = comdat any

$_ZTI11CompareableIlE = comdat any

$_ZTV11CompareableIlE = comdat any

$_ZTS6Person = comdat any

$_ZTI6Person = comdat any

$_ZTV6Person = comdat any

@_ZTV8TypeInfo = external global ptr
@_ZTS11CompareableIlE = linkonce_odr dso_local constant [14 x i8] c"11Compareable\00", comdat, align 1
@_ZTI11CompareableIlE = linkonce_odr dso_local constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i32 2), ptr @_ZTS11CompareableIlE }, comdat
@_ZTV11CompareableIlE = linkonce_odr dso_local unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI11CompareableIlE, ptr null] }, comdat
@_ZTS6Person = linkonce_odr dso_local constant [8 x i8] c"6Person\00", comdat, align 1
@_ZTI6Person = linkonce_odr dso_local constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i32 2), ptr @_ZTS6Person }, comdat
@_ZTV6Person = linkonce_odr dso_local unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI6Person, ptr @_ZN6Person7compareERKlRKl] }, comdat
@anon.string.0 = private unnamed_addr constant [5 x i8] c"Mike\00", align 1
@anon.string.1 = private unnamed_addr constant [7 x i8] c"Miller\00", align 1
@printf.str.0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

define private i32 @_ZN6Person7compareERKlRKl(ptr noundef nonnull align 8 dereferenceable(32) %0, ptr %1, ptr %2) {
  %result = alloca i32, align 4
  %this = alloca ptr, align 8
  %a = alloca ptr, align 8
  %b = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %a, align 8
  store ptr %2, ptr %b, align 8
  %4 = load ptr, ptr %b, align 8
  %5 = load i64, ptr %4, align 8
  %6 = load ptr, ptr %a, align 8
  %7 = load i64, ptr %6, align 8
  %8 = icmp sgt i64 %7, %5
  br i1 %8, label %if.then.L20, label %if.exit.L20

if.then.L20:                                      ; preds = %3
  ret i32 2

if.exit.L20:                                      ; preds = %3
  %9 = load ptr, ptr %b, align 8
  %10 = load i64, ptr %9, align 8
  %11 = load ptr, ptr %a, align 8
  %12 = load i64, ptr %11, align 8
  %13 = icmp slt i64 %12, %10
  br i1 %13, label %if.then.L21, label %if.exit.L21

if.then.L21:                                      ; preds = %if.exit.L20
  ret i32 0

if.exit.L21:                                      ; preds = %if.exit.L20
  ret i32 1
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %mike = alloca %struct.Person, align 8
  %1 = alloca i64, align 8
  %2 = alloca i64, align 8
  %isEqual = alloca i1, align 1
  store i32 0, ptr %result, align 4
  store %struct.Person { %interface.Compareable zeroinitializer, ptr @anon.string.0, ptr @anon.string.1, i32 43 }, ptr %mike, align 8
  store i64 22, ptr %1, align 8
  store i64 22, ptr %2, align 8
  %3 = call i32 @_ZN6Person7compareERKlRKl(ptr noundef nonnull align 8 dereferenceable(32) %mike, ptr %1, ptr %2)
  %4 = icmp eq i32 %3, 1
  store i1 %4, ptr %isEqual, align 1
  %5 = load i1, ptr %isEqual, align 1
  %6 = zext i1 %5 to i32
  %7 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %6)
  %8 = load i32, ptr %result, align 4
  ret i32 %8
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
