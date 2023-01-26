; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.1 = private unnamed_addr constant [24 x i8] c"Starting thread %d ...\0A\00", align 1
@str = private unnamed_addr constant [21 x i8] c"Starting threads ...\00", align 1
@str.1 = private unnamed_addr constant [20 x i8] c"Hello from original\00", align 1
@str.2 = private unnamed_addr constant [22 x i8] c"Hello from the thread\00", align 1

declare i32 @usleep(i32) local_unnamed_addr

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %i = alloca i32, align 4
  %1 = alloca { ptr }, align 8
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  store i32 1, ptr %i, align 4
  br label %for.body.L5

for.body.L5:                                      ; preds = %0, %for.body.L5
  %storemerge2 = phi i32 [ 1, %0 ], [ %6, %for.body.L5 ]
  %2 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %storemerge2)
  store ptr %i, ptr %1, align 8
  %3 = alloca ptr, align 8
  %4 = call i32 @pthread_create(ptr nonnull %3, ptr null, ptr nonnull @_thread0, ptr nonnull %1) #2
  %5 = load i32, ptr %i, align 4
  %6 = add i32 %5, 1
  store i32 %6, ptr %i, align 4
  %7 = icmp slt i32 %6, 9
  br i1 %7, label %for.body.L5, label %for.exit.L5

for.exit.L5:                                      ; preds = %for.body.L5
  %8 = call i32 @usleep(i32 1000000) #2
  %puts1 = call i32 @puts(ptr nonnull dereferenceable(1) @str.1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

define private noalias ptr @_thread0(ptr nocapture readonly %0) {
  %2 = load ptr, ptr %0, align 8
  %3 = load i32, ptr %2, align 4
  %4 = mul i32 %3, 100000
  %5 = tail call i32 @usleep(i32 %4)
  %puts = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.2)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
