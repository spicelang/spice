; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Person = type { %interface.Compareable, ptr, ptr, i32 }
%interface.Compareable = type { ptr }

$_ZTS11CompareableIlE = comdat any

$_ZTI11CompareableIlE = comdat any

$_ZTV11CompareableIlE = comdat any

$_ZTS6Person = comdat any

$_ZTI6Person = comdat any

$_ZTV6Person = comdat any

@_ZTS11CompareableIlE = dso_local constant [14 x i8] c"11Compareable\00", comdat, align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI11CompareableIlE = dso_local constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS11CompareableIlE }, comdat, align 8
@_ZTV11CompareableIlE = dso_local unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI11CompareableIlE, ptr null] }, comdat, align 8
@_ZTS6Person = dso_local constant [8 x i8] c"6Person\00", comdat, align 4
@_ZTI6Person = dso_local constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS6Person, ptr @_ZTI11CompareableIlE }, comdat, align 8
@_ZTV6Person = dso_local unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI6Person, ptr @_ZN6Person7compareERKlRKl] }, comdat, align 8
@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 4
@1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 4
@anon.string.0 = private unnamed_addr constant [5 x i8] c"Mike\00", align 4
@anon.string.1 = private unnamed_addr constant [7 x i8] c"Miller\00", align 4
@printf.str.0 = private unnamed_addr constant [3 x i8] c"%d\00", align 4

define private void @_ZN6Person4ctorEPKcPKcj(ptr noundef nonnull align 8 dereferenceable(32) %0, ptr noundef %1, ptr noundef %2, i32 noundef %3) {
  %this = alloca ptr, align 8
  %firstName = alloca ptr, align 8
  %lastName = alloca ptr, align 8
  %age = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %firstName, align 8
  store ptr %2, ptr %lastName, align 8
  store i32 %3, ptr %age, align 4
  %5 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [3 x ptr] }, ptr @_ZTV6Person, i64 0, i32 0, i32 2), ptr %5, align 8
  %6 = getelementptr inbounds nuw %struct.Person, ptr %5, i32 0, i32 1
  store ptr @0, ptr %6, align 8
  %7 = getelementptr inbounds nuw %struct.Person, ptr %5, i32 0, i32 2
  store ptr @1, ptr %7, align 8
  %8 = getelementptr inbounds nuw %struct.Person, ptr %5, i32 0, i32 3
  store i32 0, ptr %8, align 4
  %9 = load ptr, ptr %this, align 8
  %firstName.addr = getelementptr inbounds %struct.Person, ptr %9, i64 0, i32 1
  %10 = load ptr, ptr %firstName, align 8
  store ptr %10, ptr %firstName.addr, align 8
  %11 = load ptr, ptr %this, align 8
  %lastName.addr = getelementptr inbounds %struct.Person, ptr %11, i64 0, i32 2
  %12 = load ptr, ptr %lastName, align 8
  store ptr %12, ptr %lastName.addr, align 8
  %13 = load ptr, ptr %this, align 8
  %age.addr = getelementptr inbounds %struct.Person, ptr %13, i64 0, i32 3
  %14 = load i32, ptr %age, align 4
  store i32 %14, ptr %age.addr, align 4
  ret void
}

define private noundef i32 @_ZN6Person7compareERKlRKl(ptr noundef nonnull align 8 dereferenceable(32) %0, ptr noundef %1, ptr noundef %2) {
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
  br i1 %8, label %if.then.L26, label %if.exit.L26

if.then.L26:                                      ; preds = %3
  ret i32 2

if.exit.L26:                                      ; preds = %3
  %9 = load ptr, ptr %b, align 8
  %10 = load i64, ptr %9, align 8
  %11 = load ptr, ptr %a, align 8
  %12 = load i64, ptr %11, align 8
  %13 = icmp slt i64 %12, %10
  br i1 %13, label %if.then.L27, label %if.exit.L27

if.then.L27:                                      ; preds = %if.exit.L26
  ret i32 0

if.exit.L27:                                      ; preds = %if.exit.L26
  ret i32 1
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 {
  %result = alloca i32, align 4
  %mike = alloca %struct.Person, align 8
  %1 = alloca i64, align 8
  %2 = alloca i64, align 8
  %isEqual = alloca i1, align 1
  store i32 0, ptr %result, align 4
  call void @_ZN6Person4ctorEPKcPKcj(ptr noundef nonnull align 8 dereferenceable(32) %mike, ptr noundef @anon.string.0, ptr noundef @anon.string.1, i32 noundef 43)
  store i64 22, ptr %1, align 8
  store i64 22, ptr %2, align 8
  %3 = call noundef i32 @_ZN6Person7compareERKlRKl(ptr noundef nonnull align 8 dereferenceable(32) %mike, ptr noundef %1, ptr noundef %2)
  %4 = icmp eq i32 %3, 1
  store i1 %4, ptr %isEqual, align 1
  %5 = load i1, ptr %isEqual, align 1
  %6 = zext i1 %5 to i32
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %6)
  %8 = load i32, ptr %result, align 4
  ret i32 %8
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
