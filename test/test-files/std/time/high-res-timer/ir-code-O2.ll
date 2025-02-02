; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Timer = type { i64, i64, i32, ptr }

@printf.str.0 = private unnamed_addr constant [28 x i8] c"Check for %d +/-%d, got %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [22 x i8] c"All assertions passed\00", align 1
@str.1 = private unnamed_addr constant [85 x i8] c"Assertion failed: Condition 'isInRange(t.getDuration(), 10l, 3)' evaluated to false.\00", align 1
@str.2 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 't.getDuration() == 0l' evaluated to false.\00", align 1
@str.3 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition 'duration == 0l' evaluated to false.\00", align 1
@str.4 = private unnamed_addr constant [84 x i8] c"Assertion failed: Condition 'isInRange(duration, 20000l, 5000)' evaluated to false.\00", align 1

; Function Attrs: nofree nounwind
define private fastcc i1 @_Z9isInRangemmj(i64 %0, i64 range(i64 10, 20001) %1, i32 range(i32 3, 5001) %2) unnamed_addr #0 {
land.exit.L6C12:
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 %1, i32 %2, i64 %0)
  %4 = zext nneg i32 %2 to i64
  %5 = sub nsw i64 %1, %4
  %.not = icmp uge i64 %0, %5
  %6 = add nuw nsw i64 %1, %4
  %7 = icmp ule i64 %0, %6
  %land_phi = select i1 %.not, i1 %7, i1 false
  ret i1 %land_phi
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
  %t = alloca %struct.Timer, align 8
  %duration = alloca i64, align 8
  %1 = alloca %struct.Timer, align 8
  call void @_ZN5Timer4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #3
  %2 = call i64 @_ZN5Timer11getDurationEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #3
  %3 = icmp eq i64 %2, 0
  br i1 %3, label %assert.exit.L12, label %assert.then.L12, !prof !5

assert.then.L12:                                  ; preds = %0
  %puts = call i32 @puts(ptr nonnull dereferenceable(1) @str.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %0
  call void @_ZN5Timer5startEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #3
  call void @_Z5delayi(i32 10) #3
  call void @_ZN5Timer4stopEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #3
  %4 = call i64 @_ZN5Timer11getDurationEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #3
  %5 = call fastcc i1 @_Z9isInRangemmj(i64 %4, i64 10, i32 3) #3
  br i1 %5, label %assert.exit.L16, label %assert.then.L16, !prof !5

assert.then.L16:                                  ; preds = %assert.exit.L12
  %puts1 = call i32 @puts(ptr nonnull dereferenceable(1) @str.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L16:                                  ; preds = %assert.exit.L12
  store i64 0, ptr %duration, align 8
  call void @_ZN5Timer4ctorEiPm(ptr noundef nonnull align 8 dereferenceable(32) %1, i32 0, ptr nonnull %duration) #3
  %.fca.0.load = load i64, ptr %1, align 8
  %.fca.1.gep = getelementptr inbounds nuw i8, ptr %1, i64 8
  %.fca.1.load = load i64, ptr %.fca.1.gep, align 8
  %.fca.2.gep = getelementptr inbounds nuw i8, ptr %1, i64 16
  %.fca.2.load = load i32, ptr %.fca.2.gep, align 8
  %.fca.3.gep = getelementptr inbounds nuw i8, ptr %1, i64 24
  %.fca.3.load = load ptr, ptr %.fca.3.gep, align 8
  store i64 %.fca.0.load, ptr %t, align 8
  %.fca.3.insert.fca.1.gep = getelementptr inbounds nuw i8, ptr %t, i64 8
  store i64 %.fca.1.load, ptr %.fca.3.insert.fca.1.gep, align 8
  %.fca.3.insert.fca.2.gep = getelementptr inbounds nuw i8, ptr %t, i64 16
  store i32 %.fca.2.load, ptr %.fca.3.insert.fca.2.gep, align 8
  %.fca.3.insert.fca.3.gep = getelementptr inbounds nuw i8, ptr %t, i64 24
  store ptr %.fca.3.load, ptr %.fca.3.insert.fca.3.gep, align 8
  %6 = call i64 @_ZN5Timer11getDurationEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #3
  %7 = icmp eq i64 %6, 0
  br i1 %7, label %assert.exit.L21, label %assert.then.L21, !prof !5

assert.then.L21:                                  ; preds = %assert.exit.L16
  %puts2 = call i32 @puts(ptr nonnull dereferenceable(1) @str.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L16
  %8 = load i64, ptr %duration, align 8
  %9 = icmp eq i64 %8, 0
  br i1 %9, label %assert.exit.L22, label %assert.then.L22, !prof !5

assert.then.L22:                                  ; preds = %assert.exit.L21
  %puts3 = call i32 @puts(ptr nonnull dereferenceable(1) @str.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L22:                                  ; preds = %assert.exit.L21
  call void @_ZN5Timer5startEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #3
  call void @_Z5delayi(i32 10) #3
  call void @_ZN5Timer5pauseEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #3
  call void @_Z5delayi(i32 100) #3
  call void @_ZN5Timer6resumeEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #3
  call void @_Z5delayi(i32 10) #3
  call void @_ZN5Timer4stopEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #3
  %10 = load i64, ptr %duration, align 8
  %11 = call fastcc i1 @_Z9isInRangemmj(i64 %10, i64 20000, i32 5000) #3
  br i1 %11, label %assert.exit.L30, label %assert.then.L30, !prof !5

assert.then.L30:                                  ; preds = %assert.exit.L22
  %puts4 = call i32 @puts(ptr nonnull dereferenceable(1) @str.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L22
  %12 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1)
  ret i32 0
}

declare void @_ZN5Timer4ctorEv(ptr) local_unnamed_addr

declare i64 @_ZN5Timer11getDurationEv(ptr) local_unnamed_addr

; Function Attrs: cold nofree noreturn nounwind
declare void @exit(i32) local_unnamed_addr #2

declare void @_ZN5Timer5startEv(ptr) local_unnamed_addr

declare void @_Z5delayi(i32) local_unnamed_addr

declare void @_ZN5Timer4stopEv(ptr) local_unnamed_addr

declare void @_ZN5Timer4ctorEiPm(ptr, i32, ptr) local_unnamed_addr

declare void @_ZN5Timer5pauseEv(ptr) local_unnamed_addr

declare void @_ZN5Timer6resumeEv(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { cold nofree noreturn nounwind }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 2000, i32 1}
