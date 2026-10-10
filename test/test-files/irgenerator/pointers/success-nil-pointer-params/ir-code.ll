; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Inner = type { ptr, i32 }
%struct.Outer = type { ptr, i64, %struct.Inner }

@printf.str.0 = private unnamed_addr constant [30 x i8] c"Root inner parent is nil: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [38 x i8] c"Child inner parent is root inner: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [23 x i8] c"Root parent value: %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [24 x i8] c"Child parent value: %d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [18 x i8] c"Depth of nil: %d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [19 x i8] c"Depth of root: %d\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [20 x i8] c"Depth of child: %d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN5Inner4ctorEP5Inneri(ptr noundef nonnull align 8 dereferenceable(16) %0, ptr noundef align 8 %1, i32 noundef %2) #0 {
  %this = alloca ptr, align 8
  %parent = alloca ptr, align 8
  %value = alloca i32, align 4
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %parent, align 8
  store i32 %2, ptr %value, align 4
  %4 = load ptr, ptr %this, align 8
  store ptr null, ptr %4, align 8
  %5 = getelementptr inbounds nuw %struct.Inner, ptr %4, i32 0, i32 1
  store i32 0, ptr %5, align 4
  %6 = load ptr, ptr %this, align 8
  %parent.addr = getelementptr inbounds %struct.Inner, ptr %6, i64 0, i32 0
  %7 = load ptr, ptr %parent, align 8
  store ptr %7, ptr %parent.addr, align 8
  %8 = load ptr, ptr %this, align 8
  %value.addr = getelementptr inbounds %struct.Inner, ptr %8, i64 0, i32 1
  %9 = load i32, ptr %value, align 4
  store i32 %9, ptr %value.addr, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN5Outer4ctorEP5Outer(ptr noundef nonnull align 8 dereferenceable(32) %0, ptr noundef align 8 %1) #0 {
  %this = alloca ptr, align 8
  %parent = alloca ptr, align 8
  %3 = alloca %struct.Inner, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %parent, align 8
  %4 = load ptr, ptr %this, align 8
  store ptr null, ptr %4, align 8
  %5 = getelementptr inbounds nuw %struct.Outer, ptr %4, i32 0, i32 1
  store i64 0, ptr %5, align 8
  %6 = load ptr, ptr %this, align 8
  %parent.addr = getelementptr inbounds %struct.Outer, ptr %6, i64 0, i32 0
  %7 = load ptr, ptr %parent, align 8
  store ptr %7, ptr %parent.addr, align 8
  %8 = load ptr, ptr %this, align 8
  %inner.addr = getelementptr inbounds %struct.Outer, ptr %8, i64 0, i32 2
  %9 = load ptr, ptr %parent, align 8
  %10 = icmp eq ptr %9, null
  br i1 %10, label %cond.true.L20C24, label %cond.false.L20C24

cond.true.L20C24:                                 ; preds = %2
  br label %cond.exit.L20C24

cond.false.L20C24:                                ; preds = %2
  %11 = load ptr, ptr %parent, align 8
  %inner.addr1 = getelementptr inbounds %struct.Outer, ptr %11, i64 0, i32 2
  br label %cond.exit.L20C24

cond.exit.L20C24:                                 ; preds = %cond.false.L20C24, %cond.true.L20C24
  %cond.result = phi ptr [ null, %cond.true.L20C24 ], [ %inner.addr1, %cond.false.L20C24 ]
  %12 = load ptr, ptr %parent, align 8
  %13 = icmp eq ptr %12, null
  %14 = select i1 %13, i32 1, i32 2
  call void @_ZN5Inner4ctorEP5Inneri(ptr noundef nonnull align 8 dereferenceable(16) %3, ptr noundef align 8 %cond.result, i32 noundef %14)
  call void @llvm.memcpy.p0.p0.i64(ptr %inner.addr, ptr %3, i64 16, i1 false)
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #1

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_ZN5Outer14getParentValueEP5Outer(ptr noundef nonnull align 8 dereferenceable(32) %0, ptr noundef align 8 %1) #0 {
  %this = alloca ptr, align 8
  %other = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %other, align 8
  %3 = load ptr, ptr %other, align 8
  %4 = icmp eq ptr %3, null
  br i1 %4, label %if.then.L25, label %if.exit.L25

if.then.L25:                                      ; preds = %2
  ret i32 -1

if.exit.L25:                                      ; preds = %2
  %5 = load ptr, ptr %other, align 8
  %inner.addr = getelementptr inbounds %struct.Outer, ptr %5, i64 0, i32 2
  %value.addr = getelementptr inbounds %struct.Inner, ptr %inner.addr, i64 0, i32 1
  %6 = load i32, ptr %value.addr, align 4
  ret i32 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z8getDepthPK5Inner(ptr noundef align 8 %0) #0 {
  %inner = alloca ptr, align 8
  %depth = alloca i32, align 4
  store ptr %0, ptr %inner, align 8
  store i32 0, ptr %depth, align 4
  br label %while.head.L34

while.head.L34:                                   ; preds = %while.body.L34, %1
  %2 = load ptr, ptr %inner, align 8
  %3 = icmp ne ptr %2, null
  br i1 %3, label %while.body.L34, label %while.exit.L34

while.body.L34:                                   ; preds = %while.head.L34
  %4 = load i32, ptr %depth, align 4
  %5 = add nsw i32 %4, 1
  store i32 %5, ptr %depth, align 4
  %6 = load ptr, ptr %inner, align 8
  %parent.addr = getelementptr inbounds %struct.Inner, ptr %6, i64 0, i32 0
  %7 = load ptr, ptr %parent.addr, align 8
  store ptr %7, ptr %inner, align 8
  br label %while.head.L34

while.exit.L34:                                   ; preds = %while.head.L34
  %8 = load i32, ptr %depth, align 4
  ret i32 %8
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #2 {
  %root = alloca %struct.Outer, align 8
  %child = alloca %struct.Outer, align 8
  call void @_ZN5Outer4ctorEP5Outer(ptr noundef nonnull align 8 dereferenceable(32) %root, ptr noundef align 8 null)
  call void @_ZN5Outer4ctorEP5Outer(ptr noundef nonnull align 8 dereferenceable(32) %child, ptr noundef align 8 %root)
  %inner.addr = getelementptr inbounds %struct.Outer, ptr %root, i64 0, i32 2
  %parent.addr = getelementptr inbounds %struct.Inner, ptr %inner.addr, i64 0, i32 0
  %1 = load ptr, ptr %parent.addr, align 8
  %2 = icmp eq ptr %1, null
  %3 = zext i1 %2 to i32
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %3)
  %inner.addr1 = getelementptr inbounds %struct.Outer, ptr %child, i64 0, i32 2
  %parent.addr2 = getelementptr inbounds %struct.Inner, ptr %inner.addr1, i64 0, i32 0
  %inner.addr3 = getelementptr inbounds %struct.Outer, ptr %root, i64 0, i32 2
  %5 = load ptr, ptr %parent.addr2, align 8
  %6 = icmp eq ptr %5, %inner.addr3
  %7 = zext i1 %6 to i32
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %7)
  %parent.addr4 = getelementptr inbounds %struct.Outer, ptr %root, i64 0, i32 0
  %9 = load ptr, ptr %parent.addr4, align 8
  %10 = call noundef i32 @_ZN5Outer14getParentValueEP5Outer(ptr noundef nonnull align 8 dereferenceable(32) %root, ptr noundef align 8 %9)
  %11 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %10)
  %parent.addr5 = getelementptr inbounds %struct.Outer, ptr %child, i64 0, i32 0
  %12 = load ptr, ptr %parent.addr5, align 8
  %13 = call noundef i32 @_ZN5Outer14getParentValueEP5Outer(ptr noundef nonnull align 8 dereferenceable(32) %child, ptr noundef align 8 %12)
  %14 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %13)
  %15 = call noundef i32 @_Z8getDepthPK5Inner(ptr noundef align 8 null)
  %16 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %15)
  %inner.addr6 = getelementptr inbounds %struct.Outer, ptr %root, i64 0, i32 2
  %17 = call noundef i32 @_Z8getDepthPK5Inner(ptr noundef align 8 %inner.addr6)
  %18 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i32 noundef %17)
  %inner.addr7 = getelementptr inbounds %struct.Outer, ptr %child, i64 0, i32 2
  %19 = call noundef i32 @_Z8getDepthPK5Inner(ptr noundef align 8 %inner.addr7)
  %20 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i32 noundef %19)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #3 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
