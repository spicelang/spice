; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Timer = type { i64, i64, i32, ptr }

@printf.str.0 = private unnamed_addr constant [28 x i8] c"Check for %d +/-%d, got %d\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [86 x i8] c"Assertion failed: Condition 'isInRange(t.getDuration(), 10l, 3)' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [73 x i8] c"Assertion failed: Condition 't.getDuration() == 0l' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'duration == 0l' evaluated to false.\0A\00", align 4
@anon.string.4 = private unnamed_addr constant [85 x i8] c"Assertion failed: Condition 'isInRange(duration, 20000l, 5000)' evaluated to false.\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [22 x i8] c"All assertions passed\00", align 4

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

; Function Attrs: noinline norecurse nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #1 {
  %t = alloca %struct.Timer, align 8
  %duration = alloca i64, align 8
  %1 = alloca %struct.Timer, align 8
  call void @_ZN5Timer4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #2
  %2 = call noundef i64 @_ZN5Timer11getDurationEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #2
  %3 = icmp eq i64 %2, 0
  br i1 %3, label %assert.exit.L12, label %assert.then.L12, !prof !5

assert.then.L12:                                  ; preds = %0
  %4 = call ptr @__acrt_iob_func(i32 2)
  %5 = call i64 @fwrite(ptr nonnull @anon.string.2, i64 72, i64 1, ptr %4)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %0
  call void @_ZN5Timer5startEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #2
  call void @_Z5delayi(i32 noundef 10) #2
  call void @_ZN5Timer4stopEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #2
  %6 = call noundef i64 @_ZN5Timer11getDurationEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #2
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 noundef 10, i32 noundef 3, i64 noundef %6)
  %8 = add i64 %6, -7
  %land_phi.i = icmp ult i64 %8, 7
  br i1 %land_phi.i, label %assert.exit.L16, label %assert.then.L16, !prof !5

assert.then.L16:                                  ; preds = %assert.exit.L12
  %9 = call ptr @__acrt_iob_func(i32 2)
  %10 = call i64 @fwrite(ptr nonnull @anon.string.1, i64 85, i64 1, ptr %9)
  call void @exit(i32 1)
  unreachable

assert.exit.L16:                                  ; preds = %assert.exit.L12
  store i64 0, ptr %duration, align 8
  call void @_ZN5Timer4ctorEiPm(ptr noundef nonnull align 8 dereferenceable(32) %1, i32 noundef 0, ptr noundef nonnull align 8 dereferenceable(8) %duration) #2
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 8 dereferenceable(32) %t, ptr noundef nonnull align 8 dereferenceable(32) %1, i64 32, i1 false)
  %11 = call noundef i64 @_ZN5Timer11getDurationEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #2
  %12 = icmp eq i64 %11, 0
  br i1 %12, label %assert.exit.L21, label %assert.then.L21, !prof !5

assert.then.L21:                                  ; preds = %assert.exit.L16
  %13 = call ptr @__acrt_iob_func(i32 2)
  %14 = call i64 @fwrite(ptr nonnull @anon.string.2, i64 72, i64 1, ptr %13)
  call void @exit(i32 1)
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L16
  %15 = load i64, ptr %duration, align 8
  %16 = icmp eq i64 %15, 0
  br i1 %16, label %assert.exit.L22, label %assert.then.L22, !prof !5

assert.then.L22:                                  ; preds = %assert.exit.L21
  %17 = call ptr @__acrt_iob_func(i32 2)
  %18 = call i64 @fwrite(ptr nonnull @anon.string.3, i64 65, i64 1, ptr %17)
  call void @exit(i32 1)
  unreachable

assert.exit.L22:                                  ; preds = %assert.exit.L21
  call void @_ZN5Timer5startEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #2
  call void @_Z5delayi(i32 noundef 10) #2
  call void @_ZN5Timer5pauseEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #2
  call void @_Z5delayi(i32 noundef 100) #2
  call void @_ZN5Timer6resumeEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #2
  call void @_Z5delayi(i32 noundef 10) #2
  call void @_ZN5Timer4stopEv(ptr noundef nonnull align 8 dereferenceable(32) %t) #2
  %19 = load i64, ptr %duration, align 8
  %20 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 noundef 20000, i32 noundef 5000, i64 noundef %19)
  %21 = add i64 %19, -15000
  %land_phi.i2 = icmp ult i64 %21, 10001
  br i1 %land_phi.i2, label %assert.exit.L30, label %assert.then.L30, !prof !5

assert.then.L30:                                  ; preds = %assert.exit.L22
  %22 = call ptr @__acrt_iob_func(i32 2)
  %23 = call i64 @fwrite(ptr nonnull @anon.string.4, i64 84, i64 1, ptr %22)
  call void @exit(i32 1)
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L22
  %24 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1)
  ret i32 0
}

declare void @_ZN5Timer4ctorEv(ptr) local_unnamed_addr

declare i64 @_ZN5Timer11getDurationEv(ptr) local_unnamed_addr

; Function Attrs: nounwind
declare dso_local noundef ptr @__acrt_iob_func(i32 noundef) local_unnamed_addr #2

; Function Attrs: cold nofree noreturn nounwind
declare void @exit(i32) local_unnamed_addr #3

declare void @_ZN5Timer5startEv(ptr) local_unnamed_addr

declare void @_Z5delayi(i32) local_unnamed_addr

declare void @_ZN5Timer4stopEv(ptr) local_unnamed_addr

declare void @_ZN5Timer4ctorEiPm(ptr, i32, ptr) local_unnamed_addr

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #4

declare void @_ZN5Timer5pauseEv(ptr) local_unnamed_addr

declare void @_ZN5Timer6resumeEv(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i64 @fwrite(ptr noundef readonly captures(none), i64 noundef, i64 noundef, ptr noundef captures(none)) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
attributes #1 = { noinline norecurse nounwind uwtable }
attributes #2 = { nounwind }
attributes #3 = { cold nofree noreturn nounwind }
attributes #4 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
