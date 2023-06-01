; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__Thread__long = type { i64 }

@printf.str.0 = private unnamed_addr constant [33 x i8] c"Thread returned with result: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [17 x i8] c"Program finished\00", align 1
@str = private unnamed_addr constant [45 x i8] c"Started all threads. Waiting for results ...\00", align 1

; Function Attrs: nofree nosync nounwind memory(none)
define private fastcc i32 @_f__void__int__fib__int(i32 %0) unnamed_addr #0 {
  %2 = icmp slt i32 %0, 3
  br i1 %2, label %common.ret, label %if.exit.L4

common.ret.loopexit:                              ; preds = %if.exit.L4
  %3 = add i32 %7, 1
  br label %common.ret

common.ret:                                       ; preds = %common.ret.loopexit, %1
  %accumulator.tr.lcssa = phi i32 [ 1, %1 ], [ %3, %common.ret.loopexit ]
  ret i32 %accumulator.tr.lcssa

if.exit.L4:                                       ; preds = %1, %if.exit.L4
  %.tr4 = phi i32 [ %6, %if.exit.L4 ], [ %0, %1 ]
  %accumulator.tr3 = phi i32 [ %7, %if.exit.L4 ], [ 0, %1 ]
  %4 = add nsw i32 %.tr4, -1
  %5 = tail call fastcc i32 @_f__void__int__fib__int(i32 %4)
  %6 = add nsw i32 %.tr4, -2
  %7 = add i32 %5, %accumulator.tr3
  %8 = icmp ult i32 %.tr4, 5
  br i1 %8, label %common.ret.loopexit, label %if.exit.L4
}

; Function Attrs: nofree nounwind
define private void @_p__void__void__calcFib30() #1 {
  %1 = tail call fastcc i32 @_f__void__int__fib__int(i32 30)
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %1)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #2 {
for.body.L16:
  %threads = alloca [8 x %__Thread__long], align 8
  %0 = alloca %__Thread__long, align 8
  %.fca.1.0.gep = getelementptr inbounds [8 x %__Thread__long], ptr %threads, i64 0, i64 1, i32 0
  %.fca.2.0.gep = getelementptr inbounds [8 x %__Thread__long], ptr %threads, i64 0, i64 2, i32 0
  %.fca.3.0.gep = getelementptr inbounds [8 x %__Thread__long], ptr %threads, i64 0, i64 3, i32 0
  %.fca.4.0.gep = getelementptr inbounds [8 x %__Thread__long], ptr %threads, i64 0, i64 4, i32 0
  %.fca.5.0.gep = getelementptr inbounds [8 x %__Thread__long], ptr %threads, i64 0, i64 5, i32 0
  %.fca.6.0.gep = getelementptr inbounds [8 x %__Thread__long], ptr %threads, i64 0, i64 6, i32 0
  %.fca.7.0.gep = getelementptr inbounds [8 x %__Thread__long], ptr %threads, i64 0, i64 7, i32 0
  call void @"_mp__Thread__void__ctor__p()"(ptr nonnull %0, ptr nonnull @_p__void__void__calcFib30) #3
  %1 = load i64, ptr %0, align 8
  store i64 %1, ptr %threads, align 8
  call void @"_mp__Thread__void__ctor__p()"(ptr nonnull %0, ptr nonnull @_p__void__void__calcFib30) #3
  %2 = load i64, ptr %0, align 8
  store i64 %2, ptr %.fca.1.0.gep, align 8
  call void @"_mp__Thread__void__ctor__p()"(ptr nonnull %0, ptr nonnull @_p__void__void__calcFib30) #3
  %3 = load i64, ptr %0, align 8
  store i64 %3, ptr %.fca.2.0.gep, align 8
  call void @"_mp__Thread__void__ctor__p()"(ptr nonnull %0, ptr nonnull @_p__void__void__calcFib30) #3
  %4 = load i64, ptr %0, align 8
  store i64 %4, ptr %.fca.3.0.gep, align 8
  call void @"_mp__Thread__void__ctor__p()"(ptr nonnull %0, ptr nonnull @_p__void__void__calcFib30) #3
  %5 = load i64, ptr %0, align 8
  store i64 %5, ptr %.fca.4.0.gep, align 8
  call void @"_mp__Thread__void__ctor__p()"(ptr nonnull %0, ptr nonnull @_p__void__void__calcFib30) #3
  %6 = load i64, ptr %0, align 8
  store i64 %6, ptr %.fca.5.0.gep, align 8
  call void @"_mp__Thread__void__ctor__p()"(ptr nonnull %0, ptr nonnull @_p__void__void__calcFib30) #3
  %7 = load i64, ptr %0, align 8
  store i64 %7, ptr %.fca.6.0.gep, align 8
  call void @"_mp__Thread__void__ctor__p()"(ptr nonnull %0, ptr nonnull @_p__void__void__calcFib30) #3
  %8 = load i64, ptr %0, align 8
  store i64 %8, ptr %.fca.7.0.gep, align 8
  %puts = call i32 @puts(ptr nonnull dereferenceable(1) @str)
  call void @_mp__Thread__void__join(ptr nonnull %threads) #3
  call void @_mp__Thread__void__join(ptr nonnull %.fca.1.0.gep) #3
  call void @_mp__Thread__void__join(ptr nonnull %.fca.2.0.gep) #3
  call void @_mp__Thread__void__join(ptr nonnull %.fca.3.0.gep) #3
  call void @_mp__Thread__void__join(ptr nonnull %.fca.4.0.gep) #3
  call void @_mp__Thread__void__join(ptr nonnull %.fca.5.0.gep) #3
  call void @_mp__Thread__void__join(ptr nonnull %.fca.6.0.gep) #3
  call void @_mp__Thread__void__join(ptr nonnull %.fca.7.0.gep) #3
  %9 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2)
  ret i32 0
}

declare void @"_mp__Thread__void__ctor__p()"(ptr, ptr) local_unnamed_addr

declare void @_mp__Thread__void__join(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #1

attributes #0 = { nofree nosync nounwind memory(none) }
attributes #1 = { nofree nounwind }
attributes #2 = { noinline nounwind optnone uwtable }
attributes #3 = { nounwind }
