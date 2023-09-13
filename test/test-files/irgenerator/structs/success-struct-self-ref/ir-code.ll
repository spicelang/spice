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
  %1 = getelementptr inbounds %struct.TreeNode, ptr %_childNode1, i32 0, i32 0
  store ptr %rootNode, ptr %1, align 8
  %2 = getelementptr inbounds %struct.TreeNode, ptr %_childNode1, i32 0, i32 1
  store i32 1, ptr %2, align 4
  %3 = getelementptr inbounds %struct.TreeNode, ptr %childNode2, i32 0, i32 0
  store ptr %rootNode, ptr %3, align 8
  %4 = getelementptr inbounds %struct.TreeNode, ptr %childNode2, i32 0, i32 1
  store i32 2, ptr %4, align 4
  %5 = getelementptr inbounds %struct.TreeNode, ptr %childNode21, i32 0, i32 0
  store ptr %childNode2, ptr %5, align 8
  %6 = getelementptr inbounds %struct.TreeNode, ptr %childNode21, i32 0, i32 1
  store i32 3, ptr %6, align 4
  store ptr %childNode21, ptr %curNode, align 8
  br label %while.head.L21

while.head.L21:                                   ; preds = %while.body.L21, %0
  %7 = load ptr, ptr %curNode, align 8
  %parent_addr = getelementptr inbounds %struct.TreeNode, ptr %7, i32 0, i32 0
  %8 = load ptr, ptr %parent_addr, align 8
  %9 = icmp ne ptr %8, null
  br i1 %9, label %while.body.L21, label %while.exit.L21

while.body.L21:                                   ; preds = %while.head.L21
  %10 = load ptr, ptr %curNode, align 8
  %parent_addr1 = getelementptr inbounds %struct.TreeNode, ptr %10, i32 0, i32 0
  %11 = load ptr, ptr %parent_addr1, align 8
  store ptr %11, ptr %curNode, align 8
  br label %while.head.L21

while.exit.L21:                                   ; preds = %while.head.L21
  %12 = load ptr, ptr %curNode, align 8
  %value_addr = getelementptr inbounds %struct.TreeNode, ptr %12, i32 0, i32 1
  %13 = load i32, ptr %value_addr, align 4
  %14 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %13)
  %15 = load i32, ptr %result, align 4
  ret i32 %15
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
