; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.TreeNode = type { ptr, i32 }

@printf.str.0 = private unnamed_addr constant [21 x i8] c"Root node number: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %rootNode = alloca %struct.TreeNode, align 8
  %_childNode1 = alloca %struct.TreeNode, align 8
  %childNode2 = alloca %struct.TreeNode, align 8
  %childNode21 = alloca %struct.TreeNode, align 8
  %curNode = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store %struct.TreeNode zeroinitializer, ptr %rootNode, align 8
  store ptr %rootNode, ptr %_childNode1, align 8
  %1 = getelementptr inbounds %struct.TreeNode, ptr %_childNode1, i32 0, i32 1
  store i32 1, ptr %1, align 4
  store ptr %rootNode, ptr %childNode2, align 8
  %2 = getelementptr inbounds %struct.TreeNode, ptr %childNode2, i32 0, i32 1
  store i32 2, ptr %2, align 4
  store ptr %childNode2, ptr %childNode21, align 8
  %3 = getelementptr inbounds %struct.TreeNode, ptr %childNode21, i32 0, i32 1
  store i32 3, ptr %3, align 4
  store ptr %childNode21, ptr %curNode, align 8
  br label %while.head.L21

while.head.L21:                                   ; preds = %while.body.L21, %0
  %4 = load ptr, ptr %curNode, align 8
  %parent_addr = getelementptr inbounds %struct.TreeNode, ptr %4, i32 0, i32 0
  %5 = load ptr, ptr %parent_addr, align 8
  %6 = icmp ne ptr %5, null
  br i1 %6, label %while.body.L21, label %while.exit.L21

while.body.L21:                                   ; preds = %while.head.L21
  %7 = load ptr, ptr %curNode, align 8
  %parent_addr1 = getelementptr inbounds %struct.TreeNode, ptr %7, i32 0, i32 0
  %8 = load ptr, ptr %parent_addr1, align 8
  store ptr %8, ptr %curNode, align 8
  br label %while.head.L21

while.exit.L21:                                   ; preds = %while.head.L21
  %9 = load ptr, ptr %curNode, align 8
  %value_addr = getelementptr inbounds %struct.TreeNode, ptr %9, i32 0, i32 1
  %10 = load i32, ptr %value_addr, align 4
  %11 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %10)
  %12 = load i32, ptr %result, align 4
  ret i32 %12
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
