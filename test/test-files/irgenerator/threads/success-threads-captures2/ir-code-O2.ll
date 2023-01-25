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
  %capturedVariable = alloca i32, align 4
  %i = alloca i32, align 4
  %1 = alloca { ptr, ptr }, align 8
  %puts = tail call i32 @puts(ptr nonnull @str)
  store i32 0, ptr %capturedVariable, align 4
  store i32 1, ptr %i, align 4
  %2 = getelementptr inbounds { ptr, ptr }, ptr %1, i64 0, i32 1
  br label %for.body.L6

for.body.L6:                                      ; preds = %0, %for.body.L6
  %storemerge2 = phi i32 [ 1, %0 ], [ %7, %for.body.L6 ]
  %3 = call i32 (ptr, ...) @printf(ptr noundef nonnull @printf.str.1, i32 %storemerge2)
  store ptr %capturedVariable, ptr %1, align 8
  store ptr %i, ptr %2, align 8
  %4 = alloca ptr, align 8
  %5 = call i32 @pthread_create(ptr nonnull %4, ptr null, ptr nonnull @_thread0, ptr nonnull %1) #2
  %6 = load i32, ptr %i, align 4
  %7 = add i32 %6, 1
  store i32 %7, ptr %i, align 4
  %8 = icmp slt i32 %7, 9
  br i1 %8, label %for.body.L6, label %for.exit.L6

for.exit.L6:                                      ; preds = %for.body.L6
  %9 = call i32 @usleep(i32 1000000) #2
  %puts1 = call i32 @puts(ptr nonnull @str.1)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

define private noalias ptr @_thread0(ptr nocapture readonly %0) {
  %2 = load ptr, ptr %0, align 8
  %3 = getelementptr inbounds { ptr, ptr }, ptr %0, i64 0, i32 1
  %4 = load ptr, ptr %3, align 8
  %5 = load i32, ptr %4, align 4
  %6 = mul i32 %5, 100000
  %7 = tail call i32 @usleep(i32 %6)
  %8 = load i32, ptr %2, align 4
  %9 = shl i32 %8, 1
  store volatile i32 %9, ptr %2, align 4
  %puts = tail call i32 @puts(ptr nonnull @str.2)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
