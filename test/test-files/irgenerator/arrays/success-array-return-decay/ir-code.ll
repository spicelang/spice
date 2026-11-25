; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Buffer = type { [3 x i32] }

@anon.array.0 = private unnamed_addr constant [3 x i32] [i32 7, i32 8, i32 9]
@printf.str.0 = private unnamed_addr constant [12 x i8] c"%d, %d, %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@anon.array.1 = private unnamed_addr constant [3 x i32] [i32 1, i32 2, i32 3]
@printf.str.2 = private unnamed_addr constant [12 x i8] c"%d, %d, %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_ZN6Buffer4dataEv(ptr noundef nonnull align 4 dereferenceable(12) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %items.addr = getelementptr inbounds %struct.Buffer, ptr %2, i64 0, i32 0
  %3 = getelementptr inbounds [3 x i32], ptr %items.addr, i64 0, i32 0
  ret ptr %3
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_Z9firstItemA3_i(ptr noundef %items) #0 {
  %1 = getelementptr inbounds [3 x i32], ptr %items, i64 0, i32 0
  ret ptr %1
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_Z16firstItemUnsizedPi(ptr noundef %0) #0 {
  %items = alloca ptr, align 8
  store ptr %0, ptr %items, align 8
  %2 = load ptr, ptr %items, align 8
  ret ptr %2
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_Z18firstItemViaResultA3_i(ptr noundef %items) #0 {
  %result = alloca ptr, align 8
  %1 = getelementptr inbounds [3 x i32], ptr %items, i64 0, i32 0
  store ptr %1, ptr %result, align 8
  %2 = load ptr, ptr %result, align 8
  ret ptr %2
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_Z20firstItemViaVariableA3_i(ptr noundef %items) #0 {
  %first = alloca ptr, align 8
  %1 = getelementptr inbounds [3 x i32], ptr %items, i64 0, i32 0
  store ptr %1, ptr %first, align 8
  %2 = load ptr, ptr %first, align 8
  ret ptr %2
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #1 {
  %buffer = alloca %struct.Buffer, align 8
  %data = alloca ptr, align 8
  %numbers = alloca [3 x i32], align 4
  %first = alloca ptr, align 8
  store %struct.Buffer { [3 x i32] [i32 7, i32 8, i32 9] }, ptr %buffer, align 4
  %1 = call noundef ptr @_ZN6Buffer4dataEv(ptr noundef nonnull align 4 dereferenceable(12) %buffer)
  store ptr %1, ptr %data, align 8
  %2 = load ptr, ptr %data, align 8
  %3 = getelementptr inbounds i32, ptr %2, i32 0
  %4 = load i32, ptr %3, align 4
  %5 = load ptr, ptr %data, align 8
  %6 = getelementptr inbounds i32, ptr %5, i32 1
  %7 = load i32, ptr %6, align 4
  %8 = load ptr, ptr %data, align 8
  %9 = getelementptr inbounds i32, ptr %8, i32 2
  %10 = load i32, ptr %9, align 4
  %11 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %4, i32 noundef %7, i32 noundef %10)
  %items.addr = getelementptr inbounds %struct.Buffer, ptr %buffer, i64 0, i32 0
  %12 = getelementptr inbounds [3 x i32], ptr %items.addr, i64 0, i32 0
  %13 = load ptr, ptr %data, align 8
  %14 = icmp eq ptr %13, %12
  %15 = zext i1 %14 to i32
  %16 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %15)
  store [3 x i32] [i32 1, i32 2, i32 3], ptr %numbers, align 4
  %17 = call noundef ptr @_Z9firstItemA3_i(ptr noundef %numbers)
  store ptr %17, ptr %first, align 8
  %18 = load ptr, ptr %first, align 8
  %19 = getelementptr inbounds i32, ptr %18, i32 0
  %20 = load i32, ptr %19, align 4
  %21 = load ptr, ptr %first, align 8
  %22 = getelementptr inbounds i32, ptr %21, i32 1
  %23 = load i32, ptr %22, align 4
  %24 = load ptr, ptr %first, align 8
  %25 = getelementptr inbounds i32, ptr %24, i32 2
  %26 = load i32, ptr %25, align 4
  %27 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %20, i32 noundef %23, i32 noundef %26)
  %28 = getelementptr inbounds [3 x i32], ptr %numbers, i64 0, i32 0
  %29 = load ptr, ptr %first, align 8
  %30 = icmp eq ptr %29, %28
  %31 = zext i1 %30 to i32
  %32 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %31)
  %33 = getelementptr inbounds [3 x i32], ptr %numbers, i64 0, i32 0
  %34 = call noundef ptr @_Z16firstItemUnsizedPi(ptr noundef %33)
  %35 = load i32, ptr %34, align 4
  %36 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %35)
  %37 = call noundef ptr @_Z18firstItemViaResultA3_i(ptr noundef %numbers)
  %38 = load i32, ptr %37, align 4
  %39 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i32 noundef %38)
  %40 = call noundef ptr @_Z20firstItemViaVariableA3_i(ptr noundef %numbers)
  %41 = load i32, ptr %40, align 4
  %42 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i32 noundef %41)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
