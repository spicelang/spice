; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [24 x i8] c"Starting thread %d ...\0A\00", align 1
@str = private unnamed_addr constant [21 x i8] c"Starting threads ...\00", align 1
@str.1 = private unnamed_addr constant [20 x i8] c"Hello from original\00", align 1
@str.2 = private unnamed_addr constant [22 x i8] c"Hello from the thread\00", align 1

declare i32 @usleep(i32) local_unnamed_addr

define i32 @main() local_unnamed_addr {
entry.l3:
  %i = alloca i32, align 4
  %0 = alloca { ptr }, align 8
  %puts = tail call i32 @puts(ptr nonnull @str)
  store i32 1, ptr %i, align 4
  br label %for.l5

for.l5:                                           ; preds = %entry.l3, %for.l5
  %storemerge2 = phi i32 [ 1, %entry.l3 ], [ %5, %for.l5 ]
  %1 = call i32 (ptr, ...) @printf(ptr nonnull @0, i32 %storemerge2)
  store ptr %i, ptr %0, align 8
  %2 = alloca ptr, align 8
  %3 = call i32 @pthread_create(ptr nonnull %2, ptr null, ptr nonnull @_thread0, ptr nonnull %0)
  %4 = load i32, ptr %i, align 4
  %5 = add i32 %4, 1
  store i32 %5, ptr %i, align 4
  %6 = icmp slt i32 %5, 9
  br i1 %6, label %for.l5, label %for.end.l5

for.end.l5:                                       ; preds = %for.l5
  %7 = call i32 @usleep(i32 1000000)
  %puts1 = call i32 @puts(ptr nonnull @str.1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

define internal noalias ptr @_thread0(ptr nocapture readonly %0) {
thread.entry.l7:
  %1 = load ptr, ptr %0, align 8
  %2 = load i32, ptr %1, align 4
  %3 = mul i32 %2, 100000
  %4 = tail call i32 @usleep(i32 %3)
  %puts = tail call i32 @puts(ptr nonnull @str.2)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
